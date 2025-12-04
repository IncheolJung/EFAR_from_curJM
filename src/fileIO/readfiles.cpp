#include <fstream>
#include <iostream>
#include <string>

#include "typeDef.hpp"
#include "myVector.hpp"


void readIn (const std::string& fname, IN_data& inData) {
  std::ifstream file(fname);
  
  if (!file.is_open()) {
    throw std::runtime_error("[!] Could not open file: " + fname);
  }
  
  std::string header;
  std::getline(file, header);   // skip header
  
  myFloat residual, ipretype, skeleton_tol;
  
  file >> inData.size;
  file >> inData.frequency;
  file >> residual;
  file >> ipretype;
  file >> skeleton_tol;
  
  inData.resize();
  
  for (int i = 0; i<inData.size; ++i) {
    file >> inData.ID_list[i] >> inData.theta_list[i] >> inData.phi_list[i] 
    >> inData.Einc_mag_list[i] >> inData.Einc_phase_list[i] 
    >> inData.pol_list[i] >> inData.outCurJ[i];
  }
  
  file.close();
  
}


void readTri (const std::string& fname, TRI_data& triData) {
  std::ifstream file(fname);
  
  if (!file.is_open()) {
    throw std::runtime_error("[!] Could not open file: " + fname);
  }
  
  file >> triData.scale;
  
  file >> triData.node_size;
  triData.resize_node();
  for (int i = 0; i<triData.node_size; ++i) {
    file >> triData.node_data[i][0] >> triData.node_data[i][1] >> triData.node_data[i][2];
  }
  
  file >> triData.face_size;
  triData.resize_face();
  for (int i = 0; i<triData.face_size; ++i) {
    file >> triData.face_data[i][0] >> triData.face_data[i][1] >> triData.face_data[i][2];
  }
  
  file.close();
  
}


// void readCurJM(const std::string& fname, currentData& curData)
// {
//   std::ifstream file(fname);
//   if (!file.is_open()) {
//     throw std::runtime_error("[!] Could not open file: " + fname);
//   }
  
//   // ---- Step 1: load all lines (serial I/O) ----
//   std::vector<std::string> lines;
//   std::string line;
//   while (std::getline(file, line))
//   lines.push_back(std::move(line));
  
//   file.close();
  
//   int N = lines.size() / 10;       // number of current entries
//   if (curData.size() < (size_t)N) {
//     throw std::runtime_error("[!] curData not large enough for readCurJM.");
//   }
  
//   // ---- Step 2: parallel processing ----
//   #pragma omp parallel for
//   for (int g = 0; g < N; g++) {
    
//     std::array<myComplex,3> accum = {0,0,0};
    
//     // group g corresponds to lines g*10 ... g*10+9
//     for (int i = 0; i < 9; i++) {
//       int line_id = g*10 + i;
      
//       std::istringstream iss(lines[line_id]);
//       myFloat re, im;
//       iss >> re >> im;
      
//       accum[i % 3] += myComplex(re, im);
//     }
    
//     // average each component
//     for (int k = 0; k < 3; k++)
//     accum[k] /= 3.0;
    
//     curData[g] = CVec3(accum[0], accum[1], accum[2]);
//   }
// }


// int readCurJM_SGF(const std::string& fname, currentData& curData)
// {
//   std::ifstream file(fname);
//   if (!file.is_open()) {
//     std::cerr << "[!] Could not open file: " << fname << "\n";
//     return 1;
//   }
  
//   if (!curData.empty()) {
//     std::cerr << "curData must be empty but has size "
//     << curData.size() << "\n";
//     return 1;
//   }
  
//   // ---- Step 1: load all lines (serial I/O) ----
//   std::vector<std::string> lines;
//   std::string line;
//   while (std::getline(file, line))
//   lines.push_back(std::move(line));
  
//   file.close();
  
//   int N = lines.size() / 10;
//   curData.resize(N);     // make thread-safe
  
//   // ---- Step 2: parallel processing ----
//   #pragma omp parallel for
//   for (int g = 0; g < N; g++) {
    
//     std::array<myComplex,3> accum = {0,0,0};
    
//     for (int i = 0; i < 9; i++) {
//       int line_id = g*10 + i;
      
//       std::istringstream iss(lines[line_id]);
//       myFloat re, im;
//       iss >> re >> im;
      
//       accum[i % 3] += myComplex(re, im);
//     }
    
//     for (int k = 0; k < 3; k++)
//     accum[k] /= 3.0;
    
//     curData[g] = CVec3(accum[0], accum[1], accum[2]);
//   }
  
//   return 0;
// }


void readCurJM (const std::string& fname, currentData& curData) {
  std::ifstream file(fname);

  if (!file.is_open()) {
    throw std::runtime_error("[!] Could not open file: " + fname);
  }

  int num_line = 0;
  std::string line;
  std::array<myComplex, 3> cur_data = {0, 0, 0};  // x, y, z
  while (std::getline(file, line)) {
  // for (int l=0; l<20; ++l) {
  //   std::getline(file, line);
  //   std::cout << line << "\n";
    if (num_line%10==9) {
      // std::cout << cur_data[0] << " " << cur_data[1] << " " << cur_data[2] << "\n";
      for (int i=0; i<3; ++i) cur_data[i] /= 3;
      // curData.push_back(CVec3(cur_data[0], cur_data[1], cur_data[2]));
      curData[num_line/10] = CVec3(cur_data[0], cur_data[1], cur_data[2]);
      cur_data = {0, 0, 0};
    } else {
      std::istringstream iss(line);
      myFloat re, im;
      iss >> re >> im;
      cur_data[num_line%10%3] += myComplex(re, im);
      // cur_data[num_line%10/3] += myComplex(re, im);
    }
    // std::cout << cur_data[0] << " " << cur_data[1] << " " << cur_data[2] << "\n";
    num_line++;
  }

  file.close();

}


// int readCurJM_SGF (const std::string& fname, currentData& curData) {
//   std::ifstream file(fname);

//   if (!file.is_open()) {
//     std::cerr << "[!] Could not open file: " + fname << "\n";
//     return 1;
//   }

//   if (curData.size() != 0) {
//     std::cerr << "curData must be empty but has size " + curData.size() << "\n";
//     return 1;
//   }

//   int num_line = 0;
//   std::string line;
//   std::array<myComplex, 3> cur_data = {0, 0, 0};  // x, y, z
//   while (std::getline(file, line)) {
//   // for (int l=0; l<20; ++l) {
//   //   std::getline(file, line);
//   //   std::cout << line << "\n";
//     if (num_line%10==9) {
//       // std::cout << cur_data[0] << " " << cur_data[1] << " " << cur_data[2] << "\n";
//       for (int i=0; i<3; ++i) cur_data[i] /= 3;
//       curData.push_back(CVec3(cur_data[0], cur_data[1], cur_data[2]));
//       // curData[num_line/10] = CVec3(cur_data[0], cur_data[1], cur_data[2]);
//       cur_data = {0, 0, 0};
//     } else {
//       std::istringstream iss(line);
//       myFloat re, im;
//       iss >> re >> im;
//       cur_data[num_line%10%3] += myComplex(re, im);
//       // cur_data[num_line%10/3] += myComplex(re, im);
//     }
//     // std::cout << cur_data[0] << " " << cur_data[1] << " " << cur_data[2] << "\n";
//     num_line++;
//   }

//   file.close();

//   return 0;

// }

struct Complex64 {
    float re;
    float im;
};

// read binary
int readCurJM_SGF(const std::string& fname, currentData& curData)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[!] Could not open file: " << fname << "\n";
        return 1;
    }

    if (!curData.empty()) {
        std::cerr << "curData must be empty but has size " << curData.size() << "\n";
        return 1;
    }

    while (true) {
        Complex64 xyz[3];

        // Try to read 3 float = one vector
        file.read(reinterpret_cast<char*>(xyz), 3 * sizeof(Complex64));
        if (!file) {
            // Either EOF or read error
            break;
        }

        // Your data is real-only, put into complex<float>
        CVec3 v(myComplex(xyz[0].re, xyz[0].im),
                myComplex(xyz[1].re, xyz[1].im),
                myComplex(xyz[2].re, xyz[2].im));

        curData.push_back(v);
    }

    file.close();
    return 0;
}