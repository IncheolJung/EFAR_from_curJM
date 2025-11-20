#include <fstream>
#include <iostream>
#include <vector>
#include <string>


void writeFarField (const std::string& fname, std::vector<std::string>& farfieldData) {
  std::ofstream file(fname);

  if (!file.is_open()) {
    throw std::runtime_error("[!] Could not open file: " + fname);
  }

  for (const auto farfield : farfieldData)
    file << farfield << "\n";
  
  std::cout << "Monostatic Farfield is written to " << fname << "\n";
}