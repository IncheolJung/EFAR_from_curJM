#pragma once

#include <vector>
#include <array>
#include <tuple>
#include <complex>

// Precision
using myFloat = double;
using myComplex = std::complex<myFloat>;

// Physical constants
const myFloat PI = acos(-1.0);
const myFloat MU0 = 4*PI*1e-7;
const myFloat EPS0 = 8.8541878128e-12;
const myFloat ETA0 = sqrt(MU0/EPS0);
const myComplex jj(0.0,1.0);

// data structures
using vec_i = std::vector<int>;
using vec_f = std::vector<myFloat>;
using Node = std::array<myFloat, 3>;
using Face = std::array<int, 3>;
using Nodes = std::vector<Node>;
using Faces = std::vector<Face>;

struct IN_data {
  int size;
  myFloat frequency;
  vec_i ID_list;
  vec_f theta_list;   // DEG
  vec_f phi_list;     // DEG
  vec_f Einc_mag_list;
  vec_f Einc_phase_list;
  vec_f pol_list;     // DEG
  vec_i outCurJ;
  void resize();
  void resize(int size_);
};

struct TRI_data {
  myFloat scale;
  int node_size, face_size;
  Nodes node_data;
  Faces face_data;
  void resize_node();
  void resize_face();
  void resize_node(int size_);
  void resize_face(int size_);
};