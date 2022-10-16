
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

#include <Kokkos_Core.hpp>

int main( int argc, char* argv[] )
{
    int n_element = 10000000;
    int nrepeat = 10;
    #define fptype double
    #define memlayout LayoutRight


Kokkos::initialize( argc, argv );
{

for (int test = 1000; test<4e7; test=test*1.1){
n_element = test;

    #ifdef KOKKOS_ENABLE_CUDA
    #define MemSpace Kokkos::CudaSpace
    #endif
    #ifndef MemSpace
    #define MemSpace Kokkos::HostSpace
    #endif

    using ExecSpace = MemSpace::execution_space;
    using range_policy = Kokkos::RangePolicy<ExecSpace>;

    Kokkos::View<fptype*[3][3], Kokkos::memlayout, MemSpace> J( "J", n_element);
    Kokkos::View<fptype*[4][4], Kokkos::memlayout, MemSpace> A( "A", n_element);

    Kokkos::View<fptype*[3][3], Kokkos::memlayout, MemSpace>::HostMirror h_J = Kokkos::create_mirror_view( J );
    Kokkos::View<fptype*[4][4], Kokkos::memlayout, MemSpace>::HostMirror h_A = Kokkos::create_mirror_view( A );

    Kokkos::Timer timer;
    for ( int i =0; i<n_element; i++){
        h_J(i,0,0) = 3;
        h_J(i,0,1) = 1;
        h_J(i,0,2) = 1;
        h_J(i,1,0) = 1;
        h_J(i,1,1) = 3;
        h_J(i,1,2) = 1;
        h_J(i,2,0) = 1;
        h_J(i,2,1) = 1;
        h_J(i,2,2) = 3;
    }
    double t_j_init = timer.seconds();

    timer.reset();
    Kokkos::deep_copy( J, h_J );
    Kokkos::fence();
    double t_j_init_copy = timer.seconds();
    timer.reset();
    Kokkos::deep_copy( A, h_A );
    Kokkos::fence();
    double t_a_init_copy = timer.seconds();


    timer.reset();
    for ( int repeat = 0; repeat < nrepeat; repeat++ ) {

    Kokkos::parallel_for( "fem", range_policy( 0, n_element ), KOKKOS_LAMBDA ( int i ) {
        fptype C0 = J(i, 1, 1) * J(i, 2, 2) - J(i, 1, 2) * J(i, 2, 1);
        fptype C1 = J(i, 1, 2) * J(i, 2, 0) - J(i, 1, 0) * J(i, 2, 2);
        fptype C2 = J(i, 1, 0) * J(i, 2, 1) - J(i, 1, 1) * J(i, 2, 0);
        fptype inv_J_det = J(i, 0, 0) * C0 + J(i, 0, 1) * C1 + J(i, 0, 2) * C2;
        fptype d = (1./6.) / inv_J_det;
        fptype G0 = d * (J(i,0,0) * J(i,0,0) + J(i,1,0) * J(i,1,0) + J(i,2,0) * J(i,2,0));
        fptype G1 = d * (J(i,0,0) * J(i,0,1) + J(i,1,0) * J(i,1,1) + J(i,2,0) * J(i,2,1));
        fptype G2 = d * (J(i,0,0) * J(i,0,2) + J(i,1,0) * J(i,1,2) + J(i,2,0) * J(i,2,2));
        fptype G3 = d * (J(i,0,1) * J(i,0,1) + J(i,1,1) * J(i,1,1) + J(i,2,1) * J(i,2,1));
        fptype G4 = d * (J(i,0,1) * J(i,0,2) + J(i,1,1) * J(i,1,2) + J(i,2,1) * J(i,2,2));
        fptype G5 = d * (J(i,0,2) * J(i,0,2) + J(i,1,2) * J(i,1,2) + J(i,2,2) * J(i,2,2));
        A(i,0, 0) = G0;
        A(i,0, 1) = G1;
        A(i,0, 2) = G2;
        A(i,0, 3) = -G0 - G1 - G2;
        A(i,1, 0) = G1;
        A(i,1, 1) = G3;
        A(i,1, 2) = G4;
        A(i,1, 3) = -G1 - G3 - G4;
        A(i,2, 0) = G2;
        A(i,2, 1) = G4;
        A(i,2, 2) = G5;
        A(i,2, 3) = -G2 - G4 - G5;
        A(i,3, 0) = -G0 - G1 - G2;
        A(i,3, 1) = -G1 - G3 - G4;
        A(i,3, 2) = -G2 - G4 - G5;
        A(i,3, 3) = G0 + 2 * G1 + 2 * G2 + G3 + 2 * G4 + G5;
    });

    Kokkos::fence();
    }

    double t_a_compute = timer.seconds();


    timer.reset();
    Kokkos::deep_copy( h_J, J );
    Kokkos::fence();
    double t_j_end_copy = timer.seconds();
    timer.reset();
    Kokkos::deep_copy( h_A, A );
    Kokkos::fence();
    double t_a_end_copy = timer.seconds();



    double bandwidth = 1.0e-9 * (double)(n_element*(9+16)) * (double)sizeof(fptype) / t_a_compute * (double)(nrepeat);
    double gflop = 1.0e-9 * (double)72 * (double)n_element / t_a_compute * (double)(nrepeat);
    printf("n_element = %i , t_a_compute = %f seconds, bandwidth = %f GBytes/s, performance = %f GFlop/s, t_j_init = %f , t_j_init_copy = %f , t_a_init_copy = %f , t_j_end_copy = %f , t_a_end_copy = %f\n", 
    n_element, t_a_compute, bandwidth, gflop, t_j_init, t_j_init_copy, t_a_init_copy, t_j_end_copy, t_a_end_copy);

}

}

Kokkos::finalize();

return 0;

}