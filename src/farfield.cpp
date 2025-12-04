#include "utils/typeDef.hpp"
#include "utils/myVector.hpp"

#include <iostream>
#include <iomanip>


using namespace std;

void readCurJM (const std::string& fname, currentData& curData);
void readCurJM_SGF (const std::string& fname, currentData& curData);
void mergeCurrents (currentData &cur1, currentData &cur2);


// // Far-field computation
// void computeFarField(const TRI_data &triData, const IN_data &inData, const string &simname) {
//   myFloat freq = inData.frequency * 1e6;
//   myFloat omega = 2*PI*freq;
//   myFloat k_scalar = omega*sqrt(MU0*EPS0);
  
//   int Ntri = triData.face_size;
//   int Ndir = inData.size;


//   cout << string(50, '=') << "\n";
//   cout << "Initializing Monostatic Farfield Computataion" << "\n";
//   cout << string(50, '=') << "\n\n";
//   // cout << "Theta(DEG)" << " " << "Phi(DEG)" << " "
//   //      << "Ex(Re)" << " " << "Ex(Im)" << " "
//   //      << "Ey(Re)" << " " << "Ey(Im)" << " "
//   //      << "Ez(Re)" << " " << "Ez(Im)" << "\n";
//   cout << "Theta(DEG)" << " " << "Phi(DEG)" << " "
//        << "Cpol(Re)" << " " << "Cpol(Im)" << " "
//        << "Xpol(Re)" << " " << "Xpol(Im)" << "\n";
  
//   // Loop over each direction
//   for(int d=0; d<Ndir; ++d){
//     if (inData.outCurJ[d] == 0) continue;
//     // Convert theta/phi from DEG to radians
//     myFloat th = inData.theta_list[d]*PI/180.0;
//     myFloat ph = inData.phi_list[d]*PI/180.0;
    
//     // Unit vector in observation direction
//     Vec3 rhat(sin(th)*cos(ph), sin(th)*sin(ph), cos(th));

//     // read curJM
//     currentData Jtri(Ntri);
//     currentData Mtri(Ntri);
//     string fname_curJ = simname + "_Inc#" + to_string(inData.ID_list[d]) + ".curJ";
//     string fname_curM = simname + "_Inc#" + to_string(inData.ID_list[d]) + ".curM";
//     readCurJM(fname_curJ, Jtri);
//     readCurJM(fname_curM, Mtri);
    
//     // Integrals F_e and F_m
//     CVec3 Fe, Fm;
//     #pragma omp parallel for
//     for(int t=0; t<Ntri; ++t){
//       const Face& f = triData.face_data[t];
//       Vec3 p0 = Vec3(triData.node_data[f[0]][0], triData.node_data[f[0]][1], triData.node_data[f[0]][2]);
//       Vec3 p1 = Vec3(triData.node_data[f[1]][0], triData.node_data[f[1]][1], triData.node_data[f[1]][2]);
//       Vec3 p2 = Vec3(triData.node_data[f[2]][0], triData.node_data[f[2]][1], triData.node_data[f[2]][2]);
      
//       // Triangle area and centroid
//       Vec3 v1 = p1 - p0, v2 = p2 - p0;
//       Vec3 cr = cross(v1,v2);
//       myFloat area = 0.5*norm(cr);
//       Vec3 centroid = Vec3( (p0.x+p1.x+p2.x)/3.0, (p0.y+p1.y+p2.y)/3.0, (p0.z+p1.z+p2.z)/3.0 );
      
//       // Phase factor
//       myComplex phase = exp(jj * k_scalar * dot(centroid,rhat));
      
//       // Add contribution
//       Fe = Fe + Jtri[t] * (area * phase);
//       Fm = Fm + Mtri[t] * (area * phase);
//     }
    
//     // Far-field E = e^{-jkR}/(4pi R) * [ -j omega mu rhat x (rhat x Fe) - j k rhat x Fm ]
//     // For simplicity, take R=1 m
//     Vec3 rhat_unit = rhat;
//     myComplex rdotFe = dot(Fe, rhat_unit);
//     CVec3 rxrxFe(rhat_unit.x*rdotFe - Fe.x, rhat_unit.y*rdotFe - Fe.y, rhat_unit.z*rdotFe - Fe.z);
//     CVec3 rxFm = cross(CVec3(rhat_unit.x,rhat_unit.y,rhat_unit.z), Fm);
    
//     CVec3 E_complex = rxrxFe * (-jj*omega*MU0) + rxFm * (-jj*k_scalar);
//     myComplex scalar = 1.0/(4.0*PI); // R=1 m, drop e^{-jkR} phase for simplicity
//     E_complex = E_complex * scalar;
    
//     // // Output: theta phi Ex_re Ex_im Ey_re Ey_im Ez_re Ez_im
//     // cout 
//     // << fixed << setprecision(3)
//     // << inData.theta_list[d] << " " << inData.phi_list[d] << " "
//     // << std::scientific
//     // << real(E_complex.x) << " " << imag(E_complex.x) << " "
//     // << real(E_complex.y) << " " << imag(E_complex.y) << " "
//     // << real(E_complex.z) << " " << imag(E_complex.z) << "\n";

//     //----------------------------------------------------
//     // Polarization projection: Cpol/Xpol
//     //----------------------------------------------------

//     // Eθ and Eφ components in spherical basis
//     // r̂ = (sin th cos ph, sin th sin ph, cos th)
//     // θ̂ = (cos th cos ph, cos th sin ph, -sin th)
//     // φ̂ = (-sin ph, cos ph, 0)

//     // myFloat th = inData.theta_list[d] * PI / 180.0;
//     // myFloat ph = inData.phi_list[d]   * PI / 180.0;

//     Vec3 thetahat( cos(th)*cos(ph),  cos(th)*sin(ph), -sin(th) );
//     Vec3 phihat ( -sin(ph),          cos(ph),          0.0     );

//     myComplex Etheta = E_complex.x * thetahat.x +
//                       E_complex.y * thetahat.y +
//                       E_complex.z * thetahat.z;

//     myComplex Ephi   = E_complex.x * phihat.x +
//                       E_complex.y * phihat.y +
//                       E_complex.z * phihat.z;

//     //----------------------------------------------------
//     // Apply requested polarization
//     //----------------------------------------------------
//     // pol_list[d]: 0° = VV, 90° = HH
//     // general definition: pol angle ψ (deg) describes
//     //   e_pol = cosψ * θ̂ + sinψ * φ̂

//     myFloat psi_deg = inData.pol_list[d];
//     myFloat psi = psi_deg * PI / 180.0;

//     myComplex Eco =  Etheta * cos(psi) + Ephi * sin(psi);
//     myComplex Ex  = -Etheta * sin(psi) + Ephi * cos(psi);

//     //----------------------------------------------------
//     // Output: theta phi   Cpol(re/im)   Xpol(re/im)
//     //----------------------------------------------------
//     cout << fixed << setprecision(3)
//         << inData.theta_list[d] << " "
//         << inData.phi_list[d]   << " "
//         << scientific
//         << real(Eco) << " " << imag(Eco) << " "
//         << real(Ex)  << " " << imag(Ex)  << "\n";
//   }
// }


// Far-field using Reciprocity (co- and cross-pol), returns both Eco and Ex
void computeFarField(
  const TRI_data &triData, const IN_data &inData, 
  const string &simname, vector<string> &farfield_out
) {
  myFloat freq  = inData.frequency * 1e6;
  myFloat omega = 2.0 * PI * freq;
  myFloat k0    = omega * sqrt(MU0 * EPS0);

  int Ntri = triData.face_size;
  int Ndir = inData.size;

  cout << "\n" << string(50, '=') << "\n";
  cout << "Initializing Monostatic Farfield Computation\n (Adjoint measurement)" << "\n";
  cout << string(50, '=') << "\n\n";
  string header = "Theta(DEG) Phi(DEG) Cpol(Re) Cpol(Im) Xpol(Re) Xpol(Im)";
  cout << header << "\n";
  farfield_out[0] = header;

  // Precompute centroids & areas
  vector<Vec3> centroid(Ntri);
  vector<myFloat> area(Ntri);
  #pragma omp parallel for
  for (int t = 0; t < Ntri; ++t) {
    const Face &f = triData.face_data[t];
    Vec3 p0(triData.node_data[f[0]][0], triData.node_data[f[0]][1], triData.node_data[f[0]][2]);
    Vec3 p1(triData.node_data[f[1]][0], triData.node_data[f[1]][1], triData.node_data[f[1]][2]);
    Vec3 p2(triData.node_data[f[2]][0], triData.node_data[f[2]][1], triData.node_data[f[2]][2]);
    Vec3 v1 = p1 - p0, v2 = p2 - p0;
    Vec3 cr = cross(v1, v2);
    area[t] = 0.5 * norm(cr);
    centroid[t] = Vec3((p0.x + p1.x + p2.x) / 3.0,
                       (p0.y + p1.y + p2.y) / 3.0,
                       (p0.z + p1.z + p2.z) / 3.0);
  }

  // Loop over directions
  #pragma omp parallel for
  for (int d = 0; d < Ndir; ++d) {
    if (inData.outCurJ[d] == 0) continue; // skip if not requested

    myFloat th = inData.theta_list[d] * PI / 180.0;
    myFloat ph = inData.phi_list[d]   * PI / 180.0;

    // k-hat (direction of plane wave, same as observation)
    Vec3 k_hat(sin(th) * cos(ph), sin(th) * sin(ph), cos(th));

    // spherical basis vectors
    Vec3 thetahat(cos(th) * cos(ph), cos(th) * sin(ph), -sin(th));
    Vec3 phihat(-sin(ph), cos(ph), 0.0);

    // requested receive polarization (psi from inData.pol_list)
    myFloat psi = inData.pol_list[d] * PI / 180.0;
    Vec3 e_pol = thetahat * cos(psi) + phihat * sin(psi);

    // orthogonal cross-polar (90 deg rotated)
    Vec3 e_polX = thetahat * (-sin(psi)) + phihat * cos(psi);

    // corresponding H_inc = (k_hat x E_inc)/eta0 for each polarization
    Vec3 h_pol  = cross(k_hat, e_pol)  * (1.0 / ETA0);
    Vec3 h_polX = cross(k_hat, e_polX) * (1.0 / ETA0);

    // Load currents for this incidence
    currentData Jtri(Ntri), Mtri(Ntri);
    string fnameJ = simname + "_Inc#" + to_string(inData.ID_list[d]) + ".curJ";
    string fnameM = simname + "_Inc#" + to_string(inData.ID_list[d]) + ".curM";
    readCurJM(fnameJ, Jtri);
    readCurJM(fnameM, Mtri);

    // Load sgf currents for this incidence
    currentData Jtri_SGF, Mtri_SGF;
    readCurJM_SGF(fnameJ+"_sgf", Jtri_SGF);
    readCurJM_SGF(fnameM+"_sgf", Mtri_SGF);

    // Jtri += Jtri_SGF; Mtri += Mtri_SGF;
    mergeCurrents(Jtri, Jtri_SGF);
    mergeCurrents(Mtri, Mtri_SGF);

    // Reciprocity integrals for co- and cross-pol
    myComplex FF_co = 0.0;
    myComplex FF_x  = 0.0;

    myComplex local_co = 0.0;
    myComplex local_x  = 0.0;

    for (int t = 0; t < Ntri; ++t) {
      // phase factor e^{ j k0 (k_hat · r') }
      myFloat phase = k0 * dot(k_hat, centroid[t]);
      myComplex ejk = exp(jj * phase);

      // J·E_inc and M·H_inc for co-pol
      myComplex JdotE_co = dot(Jtri[t], e_pol);
      myComplex MdotH_co = dot(Mtri[t], h_pol);

      // J·E_inc and M·H_inc for x-pol
      myComplex JdotE_x  = dot(Jtri[t], e_polX);
      myComplex MdotH_x  = dot(Mtri[t], h_polX);

      // integrand: (-j ω μ0 J·E_inc - j k0 M·H_inc) * area * e^{j k rhat·r'}
      // local_co += ( -jj * omega * MU0 * JdotE_co - jj * k0 * MdotH_co ) * area[t] * ejk;
      // local_x  += ( -jj * omega * MU0 * JdotE_x  - jj * k0 * MdotH_x  ) * area[t] * ejk;
      // M is pre-sclaed (k0/(omega*MU0))
      local_co += -jj * omega * MU0 * ( JdotE_co + MdotH_co ) * area[t] * ejk;
      local_x  += -jj * omega * MU0 * ( JdotE_x  + MdotH_x  ) * area[t] * ejk;
    }

    // safe reduction into global sums
      FF_co += local_co;
      FF_x  += local_x;

    // Scale: far-field = integral / (4π)
    myComplex Esca_co = FF_co / (4.0 * PI);
    myComplex Esca_x  = FF_x  / (4.0 * PI);

    // Output co- and cross-polar complex values
    stringstream line;
    line << fixed << setprecision(3)
         << inData.theta_list[d] << " "
         << inData.phi_list[d]   << " "
         << scientific
         << real(Esca_co) << " " << imag(Esca_co) << " "
         << real(Esca_x)  << " " << imag(Esca_x);
    cout << line.str() << "\n";
    farfield_out[d+1] = line.str();
  } // end directions loop
  cout << "\n";
}
