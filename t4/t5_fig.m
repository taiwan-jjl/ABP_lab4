%%
%
loglog(num_gpu, t_a_compute_gpu, "-x", num_gpu, t_j_init, "-x", num_gpu, t_j_init_copy, "-x", ...
       num_gpu, t_a_compute_cpu, "-x", ...
       num_gpu, t_a_end_copy, "-x");
xlabel("# of tetrahedron");
ylabel("Time(sec)");
title("Task-5");

%yline(264);
%yline(46.928);
%yline(46.928/2);

legend("t-a-compute-gpu", "t-j-init", "t-j-init-copy", "t-a-compute-cpu", "t-a-end-copy");


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

