#include <iostream>

#include "include/ntruhe.h"
#include "include/lwehe.h"

#define NUMBER_OF_EXECUTIONS 1000

float function() {
    SchemeNTRU schemeNTRU;
    Ctxt_NTRU ct_res_ntru, ct_ntru_1, ct_ntru_2;
    
    clock_t start = clock();
    schemeNTRU.nand_gate(ct_res_ntru, ct_ntru_1, ct_ntru_2);
    schemeNTRU.bootstrap(ct_res_ntru);
    std::cout << "Running time MNTRU: " << float(clock()-start)/CLOCKS_PER_SEC << std::endl;

    SchemeLWE schemeLWE;
    Ctxt_LWE ct_res_lwe, ct_lwe_1, ct_lwe_2;

    start = clock();
    schemeLWE.nand_gate(ct_res_lwe, ct_lwe_1, ct_lwe_2);
    schemeLWE.bootstrap(ct_res_lwe);
    std::cout << "Running time LWE: " << float(clock()-start)/CLOCKS_PER_SEC << std::endl;
}

void average_bootstrapping_running_time() {
    float avg_time = 0.0;
    auto start = clock();

    for (int i = 0; i < NUMBER_OF_EXECUTIONS; i++) {
        function();
        avg_time += float(clock()-start)/CLOCKS_PER_SEC;
    }

    std::cout << "Average running bootstrapping time: " << avg_time/NUMBER_OF_EXECUTIONS << " seconds" << std::endl;
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
    std::cout << "MNTRU tests" << std::endl;
    std::cout << "Key switching key size: " << (sizeof(int)*parNTRU.n*parNTRU.l_ksk)/1024.0 << " MB" << std::endl;
    std::cout << "Bootstrapping key size: " <<  (sizeof(int)*parNTRU.n*static_cast<long unsigned int>(*parNTRU.l_bsk))/1024.0 << " MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << "?" << std::endl;
    std::cout << "Number of FFTs: " << "?" << std::endl;
    std::cout << "Average bootstrapping running time over 1000 executions: " << "?" << " ms" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "LWE tests" << std::endl;
    std::cout << "Key switching key size: " << (sizeof(int)*parNTRU.n*parNTRU.l_ksk)/1024.0 << "MB" << std::endl;
    std::cout << "Bootstrapping key size: " << (sizeof(int)*parNTRU.n*static_cast<long unsigned int>(*parNTRU.l_bsk))/1024.0 << "MB" << std::endl;
    std::cout << "Mutliplication on R_Q: " << "?" << std::endl;
    std::cout << "Number of FFTs: " << "?" << std::endl;
    std::cout << "Average bootstrapping running time over 1000 executions: " << "?" << " ms" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Benchmarking completed !" << std::endl;
}

int main() {
    benchmark();

    // SchemeNTRU schemeNTRU;
    // Ctxt_NTRU ct_res_ntru, ct_ntru_1, ct_ntru_2;
    
    // clock_t start = clock();
    // schemeNTRU.nand_gate(ct_res_ntru, ct_ntru_1, ct_ntru_2);
    // schemeNTRU.bootstrap(ct_res_ntru);
    // std::cout << "Running time MNTRU: " << float(clock()-start)/CLOCKS_PER_SEC << std::endl;

    return 0;
}