#include <iostream>

#include "include/ntruhe.h"
#include "include/lwehe.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>



#define NUMBER_OF_EXECUTIONS 1000

float ntru_key_switching_key_size() {
    return (sizeof(int) * parNTRU.n * parNTRU.l_ksk) / (1024.0);
}

float lwe_key_switching_key_size() {
    return (sizeof(int) * parLWE.n * parLWE.l_ksk) / (1024.0);
}

float tfhe_bootstrapping_key_size() {
    return 4*630*1024*3*log2(pow(2, 32)) / (8 * 1000.0 * 1000.0);
}

float ntru_bootstrapping_key_size() {
    // return (parNTRU.n * (parNTRU.l_bsk[0] + parNTRU.l_bsk[1]) * parNTRU.N * log2(parNTRU.q_base)) / (8 * 1000.0 * 1000.0);
    return ((parNTRU.bsk_partition[0] * parNTRU.l_bsk[0] + parNTRU.bsk_partition[1]*parNTRU.l_bsk[1]) * parNTRU.N * log2(parNTRU.q_base)) / (8 * 1000.0 * 1000.0);
}

float lwe_bootstrapping_key_size() {
    // return (parLWE.n * (parLWE.l_bsk[0] + parLWE.l_bsk[1]) * parLWE.N * log2(parLWE.q_base)) / (8 * 1000.0 * 1000.0);
    return ((parLWE.bsk_partition[0] * parLWE.l_bsk[0] + parLWE.bsk_partition[1] * parLWE.l_bsk[1]) * parLWE.N * log2(parLWE.q_base)) / (8 * 1000.0 * 1000.0);

}

int ntru_fft_multiplication() {
    int fft_number = parNTRU.bsk_partition[0] * (parNTRU.l_bsk[0] + 1) + parNTRU.bsk_partition[1] * (parNTRU.l_bsk[1] + 1);
    // std::cout << "Number of FFTs: " << fft_number << std::endl;
    return fft_number;
}

int lwe_fft_multiplication() {
    int fft_number = parLWE.bsk_partition[0] * (parLWE.l_bsk[0] + 1) + parLWE.bsk_partition[1] * (parLWE.l_bsk[1] + 1);
    // std::cout << "Number of FFTs: " << fft_number << std::endl;
    return fft_number;
}

int ntru_ring_multiplication() {
    int ring_number = 2 * (parNTRU.bsk_partition[0] * parNTRU.l_bsk[0] + parNTRU.bsk_partition[1] * parNTRU.l_bsk[1]); // CMux 
    // std::cout << "Number of ring multiplications: " << ring_number << std::endl;
    return ring_number;
}

int lwe_ring_multiplication() {
    int ring_number = (parLWE.bsk_partition[0] * parLWE.l_bsk[0] + parLWE.bsk_partition[1] * parLWE.l_bsk[1]); // Divided for LWE
    // std::cout << "Number of ring multiplications: " << ring_number << std::endl;
    return ring_number;
}

float ntru_boostrapping_running_time(int exec_num, SchemeNTRU& schemeNTRU) {
    std::cout << "\n=== NTRU Execution #" << exec_num << " ===" << std::endl;

    Ctxt_NTRU ct_res, ct1, ct2;
    schemeNTRU.encrypt(ct1, 1);
    schemeNTRU.encrypt(ct2, 0);

    clock_t start = clock();
    schemeNTRU.nand_gate(ct_res, ct1, ct2);
    float time = float(clock() - start) / CLOCKS_PER_SEC;

    std::cout << "Bootstrapping running time: " << time << " seconds" << std::endl;
    std::cout << "=== End of NTRU Execution #" << exec_num << " ===" << std::endl;

    return time;
}


float lwe_boostrapping_running_time(int exec_num, SchemeLWE& schemeLWE) {
    std::cout << "\n=== LWE Execution #" << exec_num << " ===" << std::endl;

    Ctxt_LWE ct_res, ct1, ct2;
    schemeLWE.encrypt(ct1, 1);
    schemeLWE.encrypt(ct2, 0);

    clock_t start = clock();
    schemeLWE.nand_gate(ct_res, ct1, ct2);
    float time = float(clock() - start) / CLOCKS_PER_SEC;

    std::cout << "Bootstrapping running time: " << time << " seconds" << std::endl;
    std::cout << "=== End of LWE Execution #" << exec_num << " ===" << std::endl;

    return time;
}


void create_results_directory() {
    struct stat info;
    if (stat("results", &info) != 0) {
        // Directory does not exist, create it
        mkdir("results", 0777);
    }
}

float average_bootstrapping_running_time(std::string scheme) {
    create_results_directory();

    float avg_time = 0.0;
    std::string filename = "results/" + scheme + "_bootstrapping_times.csv";

    std::ofstream outfile(filename.c_str(), std::ios::out);
    if (!outfile.is_open()) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return 0.0;
    }

    outfile << "Execution,Time(s)\n";

    if (scheme == "NTRU") {
        SchemeNTRU schemeNTRU;
        for (int i = 0; i < NUMBER_OF_EXECUTIONS; i++) {
            float t = ntru_boostrapping_running_time(i + 1, schemeNTRU);
            outfile << (i + 1) << "," << t << "\n";
            avg_time += t;
        }

    } else if (scheme == "LWE") {
        SchemeLWE schemeLWE;
        for (int i = 0; i < NUMBER_OF_EXECUTIONS; i++) {
            float t = lwe_boostrapping_running_time(i + 1, schemeLWE);
            outfile << (i + 1) << "," << t << "\n";
            avg_time += t;
        }

    } else {
        std::cerr << "Invalid scheme specified. Use 'NTRU' or 'LWE'." << std::endl;
        return 0.0;
    }

    outfile.close();

    float average = avg_time / NUMBER_OF_EXECUTIONS;

    std::ofstream avg_out(("results/" + scheme + "_average.txt").c_str(), std::ios::out);
    if (avg_out.is_open()) {
        avg_out << "Average bootstrapping time for " << scheme << ": " << average << " s\n";
        avg_out.close();
    }

    std::cout << "\nAverage running bootstrapping time for " << scheme << ": " << average << " seconds" << std::endl;

    return average;
}


void benchmark() {
    std::cout << "Benchmarking ..." << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "TFHE tests" << std::endl;
    std::cout << "Key switching key size: " << 40 << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " << 31 << " MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << 7560 << std::endl;
    std::cout << "Number of FFTs: " << 6300 << std::endl;
    std::cout << "Average bootstrapping running time over " << 1000 << " executions: " << 66 << " ms" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "MNTRU tests" << std::endl;
    std::cout << "Key switching key size: " << ntru_key_switching_key_size() << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " <<  ntru_bootstrapping_key_size() << " MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << ntru_ring_multiplication() << std::endl;
    std::cout << "Number of FFTs: " << ntru_fft_multiplication() << std::endl;
    std::cout << "Average bootstrapping running time over " << NUMBER_OF_EXECUTIONS << " executions: \n" << average_bootstrapping_running_time("NTRU") << " s \n" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "LWE tests" << std::endl;
    std::cout << "Key switching key size: " << lwe_key_switching_key_size() << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " << lwe_bootstrapping_key_size() << " MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << lwe_ring_multiplication() << std::endl;
    std::cout << "Number of FFTs: " << lwe_fft_multiplication() << std::endl;
    std::cout << "Average bootstrapping running time over " << NUMBER_OF_EXECUTIONS << " executions: \n" << average_bootstrapping_running_time("LWE") << " s \n" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Key size pourcentage: " << (1 - (lwe_key_switching_key_size() + lwe_bootstrapping_key_size()) / (40 + 31)) * 100 << "%" << std::endl;
    std::cout << "Benchmarking completed !" << std::endl;
}

int main() {
    benchmark();

    return 0;
}