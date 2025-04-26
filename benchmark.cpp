#include <iostream>

#include "include/ntruhe.h"
#include "include/lwehe.h"

#define NUMBER_OF_EXECUTIONS 1

float ntru_key_switching_key_size() {
    return (sizeof(int) * parNTRU.n * parNTRU.l_ksk) / (1024.0);
}

float lwe_key_switching_key_size() {
    return (sizeof(int) * parLWE.n * parLWE.l_ksk) / (1024.0);
}

float ntru_bootstrapping_key_size() {
    return (2 * parNTRU.n * (parNTRU.l_bsk[0] + parNTRU.l_bsk[1]) * parNTRU.N * log2(parNTRU.q_base)) / (8 * 1000.0 * 1000.0);
}

float lwe_bootstrapping_key_size() {
    return (parLWE.n * (parLWE.l_bsk[0] + parLWE.l_bsk[1]) * parLWE.N * log2(parLWE.q_base)) / (8 * 1000.0 * 1000.0);
}

int ntru_fft_multiplication() {
    int fft_number = parNTRU.bsk_partition[0] * (parNTRU.l_bsk[0] + 1) + parNTRU.bsk_partition[1] * (parNTRU.l_bsk[1] + 1);
    std::cout << "Number of FFTs: " << fft_number << std::endl;
    return fft_number;
}

int lwe_fft_multiplication() {
    int fft_number = parLWE.bsk_partition[0] * (parLWE.l_bsk[0] + 1) + parLWE.bsk_partition[1] * (parLWE.l_bsk[1] + 1);
    std::cout << "Number of FFTs: " << fft_number << std::endl;
    return fft_number;
}

int ntru_ring_multiplication() {
    int ring_number = 2 * (parNTRU.bsk_partition[0] * (parNTRU.l_bsk[0]) + parNTRU.bsk_partition[1] * (parNTRU.l_bsk[1])); // CMux 
    // std::cout << "Number of ring multiplications: " << ring_number << std::endl;
    return ring_number;
}

int lwe_ring_multiplication() {
    int ring_number = (parLWE.bsk_partition[0] * (parLWE.l_bsk[0]) + parLWE.bsk_partition[1] * (parLWE.l_bsk[1])); // Divided for LWE
    // std::cout << "Number of ring multiplications: " << ring_number << std::endl;
    return ring_number;
}

float ntru_boostrapping_running_time() {
    SchemeNTRU schemeNTRU;
    Ctxt_NTRU ct_res_ntru, ct_ntru_1, ct_ntru_2;
    
    clock_t start = clock();
    schemeNTRU.nand_gate(ct_res_ntru, ct_ntru_1, ct_ntru_2);
    std::cout << "Running time MNTRU: " << float(clock()-start)/CLOCKS_PER_SEC << " s" << std::endl;
    return float(clock()-start)/CLOCKS_PER_SEC;
}

float lwe_boostrapping_running_time() {
    SchemeLWE schemeLWE;
    Ctxt_LWE ct_res_lwe, ct_lwe_1, ct_lwe_2;

    clock_t start = clock();
    schemeLWE.nand_gate(ct_res_lwe, ct_lwe_1, ct_lwe_2);
    std::cout << "Running time LWE: " << float(clock()-start)/CLOCKS_PER_SEC << " s" << std::endl;
    return float(clock()-start)/CLOCKS_PER_SEC;
}

float average_bootstrapping_running_time(std::string scheme) {
    float avg_time = 0.0;

    for (int i = 0; i < NUMBER_OF_EXECUTIONS; i++) {

        if (scheme == "NTRU") {
            avg_time += ntru_boostrapping_running_time();
        } else if (scheme == "LWE") {
            avg_time += lwe_boostrapping_running_time();
        } else {
            std::cerr << "Invalid scheme specified. Use 'NTRU' or 'LWE'." << std::endl;
            return 0.0;
        }
    }

    std::cout << "Average running bootstrapping time for " << scheme << ": " << avg_time/NUMBER_OF_EXECUTIONS << " seconds" << std::endl;
    return avg_time / NUMBER_OF_EXECUTIONS;
}

void benchmark() {
    SchemeNTRU schemeNTRU;
    SchemeLWE schemeLWE;


    std::cout << "Benchmarking ..." << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "TFHE tests" << std::endl;
    std::cout << "Key switching key size: " << 40 << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " << 31 << " MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << 7560 << std::endl;
    std::cout << "Number of FFTs: " << 6300 << std::endl;
    std::cout << "Average bootstrapping running time over 1000 executions: " << 66 << " ms" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "MNTRU tests" << std::endl;
    std::cout << "Key switching key size: " << ntru_key_switching_key_size() << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " <<  ntru_bootstrapping_key_size() << " MB" << std::endl; // à corriger
    std::cout << "Mutliplication on R_Q: " << ntru_ring_multiplication() << std::endl;
    std::cout << "Number of FFTs: " << ntru_fft_multiplication() << std::endl;
    std::cout << "Average bootstrapping running time over 1000 executions: " << average_bootstrapping_running_time("NTRU") << " s" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "LWE tests" << std::endl;
    std::cout << "Key switching key size: " << lwe_key_switching_key_size() << "MB" << std::endl;
    std::cout << "Bootstrapping key size: " << lwe_bootstrapping_key_size() << "MB" << std::endl; // à corriger
    std::cout << "Mutliplication on R_Q: " << lwe_ring_multiplication() << std::endl;
    std::cout << "Number of FFTs: " << lwe_fft_multiplication() << std::endl;
    std::cout << "Average bootstrapping running time over 1000 executions: " << average_bootstrapping_running_time("LWE") << " s" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Benchmarking completed !" << std::endl;
}

int main() {
    benchmark();

    return 0;
}