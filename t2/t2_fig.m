%%
%
semilogx(num_cpu_double, bw_cpu_double, "-x", num_cpu_float, bw_cpu_float, "-x", num_gpu_double, bw_gpu_double, "-x", num_gpu_float, bw_gpu_float, "-x");
xlabel("# of tetrahedron");
ylabel("GBytes/sec");
title("Task-2, bandwidth");

yline(264);
yline(46.928);
yline(46.928/2);

legend("cpu-double", "cpu-float", "gpu-double", "gpu-float", "GDDR6-192bit=264GB/s", "CPU-memory-1ch=23.5GB/s", "CPU-memory-2ch=47GB/s");


%%
%
semilogx(num_cpu_double, perf_cpu_double, "-x", num_cpu_float, perf_cpu_float, "-x", num_gpu_double, perf_gpu_double, "-x", num_gpu_float, perf_gpu_float, "-x");
xlabel("# of tetrahedron");
ylabel("GFlop/sec");
title("Task-2, Performance");

yline(199.7);
%yline(12.78*1000);
yline(19.2);

legend("cpu-double", "cpu-float", "gpu-double", "gpu-float", "gpu-FP64=199.7GFlop/s", "CPU-6C=19.2GFlop/s");
