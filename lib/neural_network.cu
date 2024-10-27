#include <cuda_runtime.h>
#include <stdio.h>

__global__ void neuralNetwork(int *a, int *b, int *c, int n){
    int index = threadIdx.x; // Mendapatkan indeks thread
    if (index < n) {
        c[index] = a[index] + b[index];
    }
}

extern "C" void runKernel(){
    int n = 512;
    int size = n * sizeof(int);

    // Alokasi memori di CPU (host)
    int *a = (int *)malloc(size);
    int *b = (int *)malloc(size);
    int *c = (int *)malloc(size);

    // Inisialisasi data
    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Alokasi memori di GPU (device)
    int *d_a, *d_b, *d_c;
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    // Salin data dari CPU ke GPU
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    // Jalankan kernel dengan jumlah thread sesuai ukuran array
    neuralNetwork<<<1, n>>>(d_a, d_b, d_c, n);

    // Salin hasil dari GPU kembali ke CPU
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    // Tampilkan hasil
    printf("Hasil penjumlahan:\n");
    for (int i = 0; i < n; i++) {
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
    }

    // Bebaskan memori
    free(a);
    free(b);
    free(c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}