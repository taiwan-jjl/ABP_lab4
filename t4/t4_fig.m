%%
%
semilogx(num_cpu_left, bw_cpu_left, "-x", num_cpu_right, bw_cpu_right, "-x", num_gpu_left, bw_gpu_left, "-x", num_gpu_right, bw_gpu_right, "-x");
xlabel("# of tetrahedron");
ylabel("GBytes/sec");
title("Task-4, bandwidth");

yline(264);
yline(46.928);
yline(46.928/2);

legend("cpu-left", "cpu-right", "gpu-left", "gpu-right", "GDDR6-192bit=264GB/s", "CPU-memory-1ch=23.5GB/s", "CPU-memory-2ch=47GB/s");


%%
%
semilogx(num_cpu_left, perf_cpu_left, "-x", num_cpu_right, perf_cpu_right, "-x", num_gpu_left, perf_gpu_left, "-x", num_gpu_right, perf_gpu_right, "-x");
xlabel("# of tetrahedron");
ylabel("GFlop/sec");
title("Task-4, Performance");

yline(199.7);
%yline(12.78*1000);
yline(19.2);

legend("cpu-left", "cpu-right", "gpu-left", "gpu-right", "gpu-FP64=199.7GFlop/s", "CPU-6C=19.2GFlop/s");

