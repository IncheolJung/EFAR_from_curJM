#include <iostream>
#include <string>
#include "utils/typeDef.hpp"
#include "utils/myVector.hpp"

#include <chrono>


using namespace std;


void readIn (const string& fname, IN_data& inData);
void readTri (const string& fname, TRI_data& triData);
// void readCurJM_SGF (const std::string& fname, currentData& curData);

void computeFarField(
  const TRI_data &triData, const IN_data &inData, 
  const string &simname, vector<string> &farfield_out
);
void writeFarField (
  const std::string& fname, 
  std::vector<std::string>& farfieldData
);


int main(int argc, char* argv[]) {

  auto t_start = std::chrono::steady_clock::now();

  cout << "EFAR computation from IN TRI CURJ CURM CURJ_SGF CURM_SGF" << "\n";
  cout << "Last compile time: " << __TIME__ << " " << __DATE__ << "\n";

  if (argc != 2) {
    cout << "Usage: " << argv[0] << " NAME" << endl;
    return 1;
  }

  const string simname = argv[1];

  IN_data inData;
  const string in_fname = simname + ".in";
  readIn(in_fname, inData);

  TRI_data triData;
  const string tri_fname = simname + ".tri";
  readTri(tri_fname, triData);

  // currentData Jtri;
  // string fname_curJ = simname + ".curJ_sgf";
  // readCurJM_SGF(fname_curJ, Jtri);

  // currentData Mtri;
  // string fname_curM = simname + ".curM_sgf";
  // readCurJM_SGF(fname_curM, Mtri);

  // cout << Jtri[0].x << Jtri[0].y << Jtri[0].z << "\n";
  // cout << Jtri[1].x << Jtri[1].y << Jtri[1].z << "\n";

  vector<string> farfield_out(inData.size+1);   // data + header
  const string efar_fname = simname + ".efar";
  computeFarField(triData, inData, simname, farfield_out);
  writeFarField(efar_fname, farfield_out);

  auto t_fin = std::chrono::steady_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t_fin - t_start).count();

  cout << "Total Elapsed Time: " << elapsed << " s\n";

  return 0;
}