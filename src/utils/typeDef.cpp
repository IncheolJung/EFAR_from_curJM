#include "typeDef.hpp"


void IN_data::resize (void) {
  IN_data::resize(size);
}

void IN_data::resize (int size_) {
  size = size_;
  ID_list.resize(size_);
  theta_list.resize(size_);
  phi_list.resize(size_);
  Einc_mag_list.resize(size_);
  Einc_phase_list.resize(size_);
  pol_list.resize(size_);
  outCurJ.resize(size_);
}



void TRI_data::resize_node( void ) {
  resize_node(node_size);
}

void TRI_data::resize_node (int size_) {
  node_size = size_;
  node_data.resize(size_);
}

void TRI_data::resize_face( void ) {
  resize_face(face_size);
}

void TRI_data::resize_face (int size_) {
  face_size = size_;
  face_data.resize(size_);
}