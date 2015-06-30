/*
** plugin_parse_init_interrupt_linux.cpp for plugin_parse_init_interrupt_linux
**
** Made by joran herve
**
** Started on  Mon, 29 Jun 2015 20:02:43 joran herve
** Last update Mon, 29 Jun 2015 20:02:43 joran herve
**
** Status : Actuellement le plugin ne gere que les interruptions Intel (documentation faite)
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "Info.hpp"


#define PLUGIN_DESCR_TYPE        PLUGIN_TYPE__PARSING
#define PLUGIN_DESCR_PRIORITE    "3000"
#define PLUGIN_DESCR_NAME        "plugin_parse_init_interrupt_linux"
#define PLUGIN_DESCR_VERSION     "0"
#define PLUGIN_NAME              PLUGIN_DESCR_TYPE "." PLUGIN_DESCR_PRIORITE "." PLUGIN_DESCR_NAME "." PLUGIN_DESCR_VERSION

extern "C"
{
    const char    *PLUGIN_FUNC_GET_PLUGIN_NAME();
    int           PLUGIN_FUNC_EXECUTE(Info *info);
    const char    *PLUGIN_FUNC_APROPOS();
}



/**
** \fn int init_interrupt_linux_i386(Info *info)
** \brief Gere l'initialisation des interruptions pour linux
**
** \param info Structure contenant les infos du programme a analyser
** \return Retourne toujours 1
*/
int    init_interrupt_linux_i386(Info *info)
{
    info->interrupt.clear();

    /* Initialisation des interruptions */
    info->interrupt.add_syscall(1, "exit", "", "int");
    info->interrupt.add_syscall(2, "fork", "int");
    info->interrupt.add_syscall(3, "read", "long", "int", "char*", "long");
    info->interrupt.add_syscall(4, "write", "long", "int", "char*", "long");
    info->interrupt.add_syscall(5, "open", "int", "char*", "int", "int");
    info->interrupt.add_syscall(6, "close", "int", "int");
    info->interrupt.add_syscall(7, "waitpid", "int", "int", "int*", "int");
    info->interrupt.add_syscall(8, "creat", "int", "char*", "int");
    info->interrupt.add_syscall(9, "link", "int", "char*", "char*");

    info->interrupt.add_syscall(10, "unlink", "int", "char*");
    info->interrupt.add_syscall(11, "execve", "int", "char*", "char**", "char**");
    info->interrupt.add_syscall(12, "chdir", "int", "char*");
    info->interrupt.add_syscall(13, "time", "long", "long*");
    info->interrupt.add_syscall(14, "mknod", "int", "char*", "int", "int");
    info->interrupt.add_syscall(15, "chmod", "int", "char*", "int");
    info->interrupt.add_syscall(16, "lchown", "int", "char*int", "int");
    info->interrupt.add_syscall(17, "break", "");
    info->interrupt.add_syscall(18, "stat", "int", "char*", "stat*");
    info->interrupt.add_syscall(19, "lseek", "long", "int", "long", "int");

    info->interrupt.add_syscall(20, "getpid", "int");
    info->interrupt.add_syscall(21, "mount", "int", "char*", "char*char*", "long", "void*");
    info->interrupt.add_syscall(22, "umount", "int", "char*");
    info->interrupt.add_syscall(23, "setuid", "int", "int");
    info->interrupt.add_syscall(24, "getuid", "int");
    info->interrupt.add_syscall(25, "stime", "int", "long");
    info->interrupt.add_syscall(26, "ptrace", "long", "long", "int", "void*", "void*");
    info->interrupt.add_syscall(27, "alarm", "int", "int");
    info->interrupt.add_syscall(28, "oldfstat", "int", "int", "stat*");
    info->interrupt.add_syscall(29, "pause", "int");

    info->interrupt.add_syscall(30, "utime", "int", "char*", "utimbuf*");
    info->interrupt.add_syscall(31, "stty", "");
    info->interrupt.add_syscall(32, "gtty", "");
    info->interrupt.add_syscall(33, "access", "int", "char*", "int");
    info->interrupt.add_syscall(34, "nice", "int", "int");
    info->interrupt.add_syscall(35, "ftime", "");
    info->interrupt.add_syscall(36, "sync", "int", "int");
    info->interrupt.add_syscall(37, "kill", "int", "int", "int");
    info->interrupt.add_syscall(38, "rename", "int", "char*", "char*");
    info->interrupt.add_syscall(39, "mkdir", "int", "char*", "int");

    info->interrupt.add_syscall(40, "rmdir", "int", "char*");
    info->interrupt.add_syscall(41, "dup", "int", "int");
    info->interrupt.add_syscall(42, "pipe", "int", "int*");
    info->interrupt.add_syscall(43, "times", "long", "tms*");
    info->interrupt.add_syscall(44, "prof", "");
    info->interrupt.add_syscall(45, "brk", "int", "void*");
    info->interrupt.add_syscall(46, "setgid", "int", "int");
    info->interrupt.add_syscall(47, "getgid", "int");
    info->interrupt.add_syscall(48, "signal", "void*", "int", "void*");
    info->interrupt.add_syscall(49, "geteuid", "int");

    info->interrupt.add_syscall(50, "getegid", "int");
    info->interrupt.add_syscall(51, "acct", "int", "char*");
    info->interrupt.add_syscall(52, "umount2", "int", "char*", "int");
    info->interrupt.add_syscall(53, "lock", "");
    info->interrupt.add_syscall(54, "ioctl", "int", "int", "int", "...");
    info->interrupt.add_syscall(55, "fcntl", "int", "int", "int", "...");
    info->interrupt.add_syscall(56, "mpx", "");
    info->interrupt.add_syscall(57, "setpgid", "int", "int", "int");
    info->interrupt.add_syscall(58, "ulimit", "");
    info->interrupt.add_syscall(59, "oldolduname", "int", "utsname*");

    info->interrupt.add_syscall(60, "umask", "int", "int");
    info->interrupt.add_syscall(61, "chroot", "int", "char*");
    info->interrupt.add_syscall(62, "ustat", "int", "long", "ustat*");
    info->interrupt.add_syscall(63, "dup2", "int", "int", "int");
    info->interrupt.add_syscall(64, "getppid", "int");
    info->interrupt.add_syscall(65, "getpgrp", "int");
    info->interrupt.add_syscall(66, "setsid", "int");
    info->interrupt.add_syscall(67, "sigaction", "int", "int", "sigaction*", "sigaction*");
    info->interrupt.add_syscall(68, "sgetmask", "long");
    info->interrupt.add_syscall(69, "ssetmask", "long", "long");

    info->interrupt.add_syscall(70, "setreuid", "int", "int", "int");
    info->interrupt.add_syscall(71, "setregid", "int", "int", "int");
    info->interrupt.add_syscall(72, "sigsuspend", "int", "sigset_t*");
    info->interrupt.add_syscall(73, "sigpending", "int", "sigset_t*");
    info->interrupt.add_syscall(74, "sethostname", "int", "char*", "long");
    info->interrupt.add_syscall(75, "setrlimit", "int", "int", "rlimit*");
    info->interrupt.add_syscall(76, "getrlimit", "int", "int", "rlimit*");
    info->interrupt.add_syscall(77, "getrusage", "int", "int", "rusage*");
    info->interrupt.add_syscall(78, "gettimeofday", "int", "timeval*", "timezone*");
    info->interrupt.add_syscall(79, "settimeofday", "int", "timeval*", "timezone*");

    info->interrupt.add_syscall(80, "getgroups", "int", "int", "int*");
    info->interrupt.add_syscall(81, "setgroups", "int", "int", "int*");
    info->interrupt.add_syscall(82, "select", "int", "fd_set*", "fd_set*", "fd_set*", "timeval*");
    info->interrupt.add_syscall(83, "symlink", "int", "char*", "char*");
    info->interrupt.add_syscall(84, "oldlstat", "int", "char*", "stat*");
    info->interrupt.add_syscall(85, "readlink", "long", "char*", "char*", "long");
    info->interrupt.add_syscall(86, "uselib", "int", "char*");
    info->interrupt.add_syscall(87, "swapon", "int", "char*", "int");
    info->interrupt.add_syscall(88, "reboot", "int", "int", "int", "int", "void*");
    info->interrupt.add_syscall(89, "readdir", "int", "int", "old_linux_dirent*", "int");

    info->interrupt.add_syscall(90, "mmap", "void*", "void*", "long", "int", "int", "int", "long");
    info->interrupt.add_syscall(91, "munmap", "int", "void*", "long");
    info->interrupt.add_syscall(92, "truncate", "int", "char*", "long");
    info->interrupt.add_syscall(93, "ftruncate", "int", "int", "long");
    info->interrupt.add_syscall(94, "fchmod", "int", "int", "int");
    info->interrupt.add_syscall(95, "fchown", "int", "int", "int", "int");
    info->interrupt.add_syscall(96, "getpriority", "int", "int", "int");
    info->interrupt.add_syscall(97, "setpriority", "int", "int", "int", "int");
    info->interrupt.add_syscall(98, "profil", "");
    info->interrupt.add_syscall(99, "statfs", "int", "char*", "statfs*");

    info->interrupt.add_syscall(100, "fstatfs", "int", "int", "statfs*");
    info->interrupt.add_syscall(101, "ioperm", "int", "long", "int");
    info->interrupt.add_syscall(102, "socketcall", "int", "int", "long*");
    info->interrupt.add_syscall(103, "syslog", "int", "int", "char*", "int");
    info->interrupt.add_syscall(104, "setitimer", "int", "int", "itimerval*", "itimerval*");
    info->interrupt.add_syscall(105, "getitimer", "int", "int", "itimerval*");
    info->interrupt.add_syscall(106, "stat", "int", "char*", "stat*");
    info->interrupt.add_syscall(107, "lstat", "int", "char*", "stat*");
    info->interrupt.add_syscall(108, "fstat", "int", "int", "stat*");
    info->interrupt.add_syscall(109, "olduname", "int", "utsname*");

    info->interrupt.add_syscall(110, "iopl", "int", "int");
    info->interrupt.add_syscall(111, "vhangup", "int");
    info->interrupt.add_syscall(112, "idle", "int");
    info->interrupt.add_syscall(113, "vm86old", "int", "vm86_struct*");
    info->interrupt.add_syscall(114, "wait4", "int", "int*", "int", "rusage*");
    info->interrupt.add_syscall(115, "swapoff", "int", "char*");
    info->interrupt.add_syscall(116, "sysinfo", "int", "sysinfo*");
    info->interrupt.add_syscall(117, "ipc", "int", "int", "int", "int", "int", "void*", "long");
    info->interrupt.add_syscall(118, "fsync", "int", "int");
    info->interrupt.add_syscall(119, "sigreturn", "int", "long");

    info->interrupt.add_syscall(120, "clone", "int", "void*", "void*", "int", "void*", "...");
    info->interrupt.add_syscall(121, "setdomainname", "int", "char*", "long");
    info->interrupt.add_syscall(122, "uname", "int", "utsname*");
    info->interrupt.add_syscall(123, "modify_ldt", "int", "int", "void*", "long");
    info->interrupt.add_syscall(124, "adjtimex", "int", "timex*");
    info->interrupt.add_syscall(125, "mprotect", "int", "void*", "long", "int");
    info->interrupt.add_syscall(126, "sigprocmask", "int", "int", "sigset_t*", "sigset_t*");
    info->interrupt.add_syscall(127, "create_module", "long", "char*", "long");
    info->interrupt.add_syscall(128, "init_module", "int", "void*", "long", "char*");
    info->interrupt.add_syscall(129, "delete_module", "int", "char*", "int");

    info->interrupt.add_syscall(130, "get_kernel_syms", "int", "kernel_sym*");
    info->interrupt.add_syscall(131, "quotactl", "int", "int", "char*", "int", "caddr_t");
    info->interrupt.add_syscall(132, "getpgid", "int", "int");
    info->interrupt.add_syscall(133, "fchdir", "int", "int");
    info->interrupt.add_syscall(134, "bdflush", "int", "int", "long");
    info->interrupt.add_syscall(135, "sysfs", "int", "int", "int", "char*");
    info->interrupt.add_syscall(136, "personality", "int", "long");
    info->interrupt.add_syscall(137, "afs_syscall", "int", "...");
    info->interrupt.add_syscall(138, "setfsuid", "int", "int");
    info->interrupt.add_syscall(139, "setfsgid", "int", "int");

    info->interrupt.add_syscall(140, "_llseek", "int", "int", "long", "long", "loff_t*", "int");
    info->interrupt.add_syscall(141, "getdents", "int", "int", "inux_dirent*", "int");
    info->interrupt.add_syscall(142, "_newselect", "int", "int", "fd_set*", "fd_set*", "fd_set*", "timeval*");
    info->interrupt.add_syscall(143, "flock", "int", "int", "int");
    info->interrupt.add_syscall(144, "msync", "int", "void*", "long", "int");
    info->interrupt.add_syscall(145, "readv", "long", "int", "iovec*", "int");
    info->interrupt.add_syscall(146, "writev", "long", "int", "iovec*", "int");
    info->interrupt.add_syscall(147, "getsid", "int", "int");
    info->interrupt.add_syscall(148, "fdatasync", "int", "int");
    info->interrupt.add_syscall(149, "_sysctl", "int", "__sysctl_args*");

    info->interrupt.add_syscall(150, "mlock", "int", "char*", "long");
    info->interrupt.add_syscall(151, "munlock", "int", "void*", "long");
    info->interrupt.add_syscall(152, "mlockall", "int", "int");
    info->interrupt.add_syscall(153, "munlockall", "int");
    info->interrupt.add_syscall(154, "sched_setparam", "int", "int", "sched_param*");
    info->interrupt.add_syscall(155, "sched_getparam", "int", "int", "sched_param*");
    info->interrupt.add_syscall(156, "sched_setscheduler", "int", "int", "int", "sched_setscheduler*");
    info->interrupt.add_syscall(157, "sched_getscheduler", "int", "int");
    info->interrupt.add_syscall(158, "sched_yield", "int");
    info->interrupt.add_syscall(159, "sched_get_priority_max", "int", "int");

    info->interrupt.add_syscall(160, "sched_get_priority_min", "int", "int");
    info->interrupt.add_syscall(161, "sched_rr_get_interval", "int", "int", "timespec*");
    info->interrupt.add_syscall(162, "nanosleep", "int", "timespec*", "timespec*");
    info->interrupt.add_syscall(163, "mremap", "void*", "void*", "long", "long", "int", "...");
    info->interrupt.add_syscall(164, "setresuid", "int", "int", "int", "int");
    info->interrupt.add_syscall(165, "getresuid", "int", "int", "int", "int");
    info->interrupt.add_syscall(166, "vm86", "int", "long", "vm86plus_struct*");
    info->interrupt.add_syscall(167, "query_module", "");
    info->interrupt.add_syscall(168, "poll", "int", "pollfd*", "nfds_t", "int");
    info->interrupt.add_syscall(169, "nfsservctl", "long", "int", "nfsctl_arg*", "nfsctl_res*");

    info->interrupt.add_syscall(170, "setresgid", "int", "int", "int", "int");
    info->interrupt.add_syscall(171, "getresgid", "int", "int", "int", "int");
    info->interrupt.add_syscall(172, "prctl", "int", "int", "long", "long", "long", "long");
    info->interrupt.add_syscall(173, "rt_sigreturn", "int", "long");
    info->interrupt.add_syscall(174, "rt_sigaction", "int", "int", "sigaction*", "sigaction*");
    info->interrupt.add_syscall(175, "rt_sigprocmask", "int", "int", "sigset_t*", "sigset_t*");
    info->interrupt.add_syscall(176, "rt_sigpending", "int", "sigset_t*");
    info->interrupt.add_syscall(177, "rt_sigtimedwait", "int", "sigset_t*", "siginfo_t*", "timespec*");
    info->interrupt.add_syscall(178, "rt_sigqueueinfo", "int", "sigset_t*", "siginfo_t*");
    info->interrupt.add_syscall(179, "rt_sigsuspend", "int", "sigset_t*");

    info->interrupt.add_syscall(180, "pread64", "long", "int", "void*", "long", "long");
    info->interrupt.add_syscall(181, "pwrite64", "long", "int", "void*", "long", "long");
    info->interrupt.add_syscall(182, "chown", "int", "char*", "int", "int");
    info->interrupt.add_syscall(183, "getcwd", "char*", "char*", "long");
    info->interrupt.add_syscall(184, "capget", "int", "cap_user_header_t", "cap_user_data_t");
    info->interrupt.add_syscall(185, "capset", "int", "cap_user_header_t", "cap_user_data_t");
    info->interrupt.add_syscall(186, "sigaltstack", "int", "stack_t*", "stack_t*");
    info->interrupt.add_syscall(187, "sendfile", "long", "int", "int", "long*", "long");
    info->interrupt.add_syscall(188, "getpmsg", "");
    info->interrupt.add_syscall(189, "putpmsg", "");

    info->interrupt.add_syscall(190, "vfork", "int");
    info->interrupt.add_syscall(191, "ugetrlimit", "int", "int", "rlimit*");
    info->interrupt.add_syscall(192, "mmap2", "void*", "void*", "long", "int", "int", "int", "long");
    info->interrupt.add_syscall(193, "truncate64", "int", "char*", "long");
    info->interrupt.add_syscall(194, "ftruncate64", "int", "int", "long");
    info->interrupt.add_syscall(195, "stat64", "int", "char*", "stat*");
    info->interrupt.add_syscall(196, "lstat64", "int", "char*", "stat*");
    info->interrupt.add_syscall(197, "fstat64", "int", "int", "stat*");
    info->interrupt.add_syscall(198, "lchown32", "int", "char*", "int", "int");
    info->interrupt.add_syscall(199, "getuid32", "int");

    info->interrupt.add_syscall(200, "getgid32", "int");
    info->interrupt.add_syscall(201, "geteuid32", "int");
    info->interrupt.add_syscall(202, "getegid32", "int");
    info->interrupt.add_syscall(203, "setreuid32", "int", "int", "int");
    info->interrupt.add_syscall(204, "setregid32", "int", "int", "int");
    info->interrupt.add_syscall(205, "getgroups32", "int", "int", "int*");
    info->interrupt.add_syscall(206, "setgroups32", "int", "int", "int*");
    info->interrupt.add_syscall(207, "fchown32", "int", "int", "int", "int");
    info->interrupt.add_syscall(208, "setresuid32", "int", "int", "int", "int");
    info->interrupt.add_syscall(209, "getresuid32", "int", "int", "int", "int");

    info->interrupt.add_syscall(210, "setresgid32", "int", "int", "int", "int");
    info->interrupt.add_syscall(211, "getresgid32", "int", "int", "int", "int");
    info->interrupt.add_syscall(212, "chown32", "int", "char*", "int", "int");
    info->interrupt.add_syscall(213, "setuid32", "int", "int");
    info->interrupt.add_syscall(214, "setgid32", "int", "int");
    info->interrupt.add_syscall(215, "setfsuid32", "int", "int");
    info->interrupt.add_syscall(216, "setfsgid32", "int", "int");
    info->interrupt.add_syscall(217, "pivot_root", "int", "char*", "char*");
    info->interrupt.add_syscall(218, "mincore", "int", "void*", "long", "char*");
    info->interrupt.add_syscall(219, "madvise", "int", "void*", "long", "int");

    info->interrupt.add_syscall(220, "getdents64", "int", "int", "linux_dirent*", "int");
    info->interrupt.add_syscall(221, "fcntl64", "int", "int", "int", "...");
    info->interrupt.add_syscall(224, "gettid", "int");
    info->interrupt.add_syscall(225, "readahead", "long", "int", "long", "long");
    info->interrupt.add_syscall(226, "setxattr", "");
    info->interrupt.add_syscall(227, "lsetxattr", "");
    info->interrupt.add_syscall(228, "fsetxattr", "");
    info->interrupt.add_syscall(229, "getxattr", "");

    info->interrupt.add_syscall(230, "lgetxattr", "");
    info->interrupt.add_syscall(231, "fgetxattr", "");
    info->interrupt.add_syscall(232, "listxattr", "");
    info->interrupt.add_syscall(233, "llistxattr", "");
    info->interrupt.add_syscall(234, "flistxattr", "");
    info->interrupt.add_syscall(235, "removexattr", "");
    info->interrupt.add_syscall(236, "lremovexattr", "");
    info->interrupt.add_syscall(237, "fremovexattr", "");
    info->interrupt.add_syscall(238, "tkill", "int", "int", "int");
    info->interrupt.add_syscall(239, "sendfile64", "long", "int", "int", "long*", "long");

    info->interrupt.add_syscall(240, "futex", "int", "int*", "int", "int", "timespec*", "int*", "int");
    info->interrupt.add_syscall(241, "sched_setaffinity", "int", "int", "long", "cpu_set_t*");
    info->interrupt.add_syscall(242, "sched_getaffinity", "int", "int", "long", "cpu_set_t*");
    info->interrupt.add_syscall(243, "set_thread_area", "int", "user_desc*");
    info->interrupt.add_syscall(244, "get_thread_area", "int", "user_desc*");
    info->interrupt.add_syscall(245, "io_setup", "int", "int", "aio_context_t*");
    info->interrupt.add_syscall(246, "io_destroy", "int", "aio_context_t*");
    info->interrupt.add_syscall(247, "io_getevents", "int", "aio_context_t", "long", "long", "io_event*", "timespec*");
    info->interrupt.add_syscall(248, "io_submit", "int", "aio_context_t", "long", "iocb**");
    info->interrupt.add_syscall(249, "io_cancel", "int", "aio_context_t*", "iocb*", "io_event*");

    info->interrupt.add_syscall(250, "fadvise64", "int", "int", "long", "long", "int");
    info->interrupt.add_syscall(251, "exit_group", "void", "int");
    info->interrupt.add_syscall(252, "lookup_dcookie", "int", "long", "char*", "long");
    info->interrupt.add_syscall(253, "epoll_create", "int", "int");
    info->interrupt.add_syscall(254, "epoll_ctl", "int", "int", "int", "int", "epoll_event*");
    info->interrupt.add_syscall(255, "epoll_wait", "int", "int", "epoll_wait*", "int", "int");
    info->interrupt.add_syscall(256, "remap_file_pages", "int", "void*", "long", "int", "long", "int");
    info->interrupt.add_syscall(257, "set_tid_address", "long", "int*");
    info->interrupt.add_syscall(258, "timer_create", "int", "clockid_t", "sigevent*", "timer_t*");
    info->interrupt.add_syscall(259, "timer_settime", "int", "timer_t", "int", "itimerspec*", "itimerspec*");

    info->interrupt.add_syscall(260, "timer_gettime", "int", "timer_t", "itimerspec*");
    info->interrupt.add_syscall(261, "timer_getoverrun", "int", "timer_t");
    info->interrupt.add_syscall(262, "timer_delete", "int", "timer_t");
    info->interrupt.add_syscall(263, "clock_settime", "int", "timer_t", "int", "itimerspec*");
    info->interrupt.add_syscall(264, "clock_gettime", "int", "timer_t", "itimerspec*");
    info->interrupt.add_syscall(265, "clock_getres", "int", "clockid_t", "timespec*");
    info->interrupt.add_syscall(267, "clock_nanosleep", "");
    info->interrupt.add_syscall(268, "statfs64", "int", "char*", "statfs*");
    info->interrupt.add_syscall(269, "fstatfs64", "int", "int", "statfs*");

    info->interrupt.add_syscall(270, "tgkil", "");
    info->interrupt.add_syscall(271, "utimes", "int", "char*", "timeval");
    info->interrupt.add_syscall(272, "fadvise64_64", "int", "int", "long", "long", "int");
    info->interrupt.add_syscall(273, "vserver", "");
    info->interrupt.add_syscall(274, "mbind", "int", "void*", "long", "int", "long*", "long", "long");
    info->interrupt.add_syscall(275, "get_mempolicy", "int", "int*", "long*", "long", "long", "long");
    info->interrupt.add_syscall(276, "set_mempolicy", "");
    info->interrupt.add_syscall(277, "mq_open", "mqd_t", "char*", "int");
    info->interrupt.add_syscall(278, "mq_unlink", "int", "char*");
    info->interrupt.add_syscall(279, "mq_timedsend", "int", "mdq_t", "char*", "long", "int");

    info->interrupt.add_syscall(280, "mq_timedreceive", "long", "mqd_t", "char*", "long", "int*");
    info->interrupt.add_syscall(281, "mq_notify", "int", "mqd_t", "sigevent*");
    info->interrupt.add_syscall(282, "mq_getsetattr", "int", "mqd_t", "mq_attr*", "mq_attr*");
    info->interrupt.add_syscall(283, "kexec_load", "long", "long", "long", "kexec_segment*", "long");
    info->interrupt.add_syscall(284, "waitid", "int", "idtype_t", "id_t", "siginfo_t*", "int");
    info->interrupt.add_syscall(286, "add_key", "key_serial_t", "char*", "char*", "void*", "long", "key_serial_t");
    info->interrupt.add_syscall(287, "request_key", "key_serial_t", "char*", "char*", "char*", "key_serial_t");
    info->interrupt.add_syscall(288, "keyctl", "long", "cmd", "...");
    info->interrupt.add_syscall(289, "ioprio_set", "int", "int", "int", "int");

    info->interrupt.add_syscall(290, "ioprio_get", "int", "int", "int");
    info->interrupt.add_syscall(291, "inotify_init", "int");
    info->interrupt.add_syscall(292, "inotify_add_watch", "int", "int", "char*", "int");
    info->interrupt.add_syscall(293, "inotify_rm_watch", "int", "int", "int");
    info->interrupt.add_syscall(294, "migrate_pages", "long", "ing", "long", "long*", "long*");
    info->interrupt.add_syscall(295, "openat", "int", "int", "char*", "int", "int");
    info->interrupt.add_syscall(296, "mkdirat", "int", "int", "char*", "int");
    info->interrupt.add_syscall(297, "mknodat", "int", "int", "char*", "int", "dev_t");
    info->interrupt.add_syscall(298, "fchownat", "int", "int", "char*", "int", "int", "int");
    info->interrupt.add_syscall(299, "futimesat", "int", "int", "char*", "timeval*");

    info->interrupt.add_syscall(300, "fstatat64", "int", "int", "char*", "stat*", "int");
    info->interrupt.add_syscall(301, "unlinkat", "int", "int", "char*", "int");
    info->interrupt.add_syscall(302, "renameat", "int", "int", "char*", "int", "char*");
    info->interrupt.add_syscall(303, "linkat", "int", "int", "char*", "int", "char*", "int");
    info->interrupt.add_syscall(304, "symlinkat", "int", "char*", "int", "char*");
    info->interrupt.add_syscall(305, "readlinkat", "int", "int", "char*", "char*", "long");
    info->interrupt.add_syscall(306, "fchmodat", "int", "int", "char*", "int", "int");
    info->interrupt.add_syscall(307, "faccessat", "int", "int", "char*", "int", "int");
    info->interrupt.add_syscall(308, "pselect6", "int", "int", "fd_set*", "fd_set*", "fd_set*", "timespec*", "sigset_t*");
    info->interrupt.add_syscall(309, "ppoll", "int", "pollfd*", "nfds_t", "int");

    info->interrupt.add_syscall(310, "unshare", "int", "int");
    info->interrupt.add_syscall(311, "set_robust_list", "long", "robust_list_head*", "mong");
    info->interrupt.add_syscall(312, "get_robust_list", "long", "int", "robust_list_head**");
    info->interrupt.add_syscall(313, "splice", "long", "int", "off_t*", "int", "off_t*", "int");
    info->interrupt.add_syscall(314, "sync_file_range", "int", "int", "long", "long", "int");
    info->interrupt.add_syscall(315, "tee", "long", "int", "int", "long", "int");
    info->interrupt.add_syscall(316, "vmsplice", "long", "int", "iovec*", "long", "int");
    info->interrupt.add_syscall(317, "move_pages", "int", "int", "long", "void**", "int*", "int*", "int*");
    info->interrupt.add_syscall(318, "getcpu", "int", "int*", "int*", "getcpu_cache*");
    info->interrupt.add_syscall(319, "epoll_pwait", "int", "inint", "int", "char*", "t", "epoll_event*", "int", "int", "sigset_t*");

    info->interrupt.add_syscall(320, "utimensat", "int", "int", "char*", "timespec*", "int");
    info->interrupt.add_syscall(321, "signalfd", "int", "int", "sigset_t*", "int");
    info->interrupt.add_syscall(322, "timerfd_create", "int", "int", "int");
    info->interrupt.add_syscall(323, "eventfd", "int", "int", "int");
    info->interrupt.add_syscall(324, "fallocate", "int", "int", "int", "long", "long");
    info->interrupt.add_syscall(325, "timerfd_settime", "int", "int", "int", "itimerspec*", "itimerspec*");
    info->interrupt.add_syscall(326, "timerfd_gettime", "int", "int", "itimerspec*");
    info->interrupt.add_syscall(327, "signalfd4", "int", "int", "sigset_t*", "int");
    info->interrupt.add_syscall(328, "eventfd2", "int", "int", "int");
    info->interrupt.add_syscall(329, "epoll_create1", "int", "int");

    info->interrupt.add_syscall(330, "dup3", "int", "int", "int", "int");
    info->interrupt.add_syscall(331, "pipe2", "int", "int*", "int");
    info->interrupt.add_syscall(332, "inotify_init1", "int", "int");
    info->interrupt.add_syscall(333, "preadv", "long", "int", "iovec*", "int", "long");
    info->interrupt.add_syscall(334, "pwritev", "long", "int", "iovec*", "int", "long");
    info->interrupt.add_syscall(335, "rt_tgsigqueueinfo", "int", "int", "int", "int", "siginfo_t*");
    info->interrupt.add_syscall(336, "perf_event_open", "");
    info->interrupt.add_syscall(337, "recvmmsg", "int", "int", "mmsghdr*", "int", "int", "timespec*");
    info->interrupt.add_syscall(338, "fanotify_init", "");
    info->interrupt.add_syscall(339, "fanotify_mark", "");

    info->interrupt.add_syscall(340, "prlimit64", "int", "int", "int", "rlimit*", "rlimit*");
    info->interrupt.add_syscall(341, "name_to_handle_at", "");
    info->interrupt.add_syscall(342, "open_by_handle_at", "");
    info->interrupt.add_syscall(343, "clock_adjtime", "");
    info->interrupt.add_syscall(344, "syncfs", "int", "int");
    info->interrupt.add_syscall(345, "sendmmsg", "int", "int", "mmsghdr*", "int", "int");
    info->interrupt.add_syscall(346, "setns", "int", "int", "int");
    info->interrupt.add_syscall(347, "process_vm_readv", "long", "int", "iovec*", "long", "iovec*", "long", "long");
    info->interrupt.add_syscall(348, "process_vm_writev", "long", "int", "iovec*", "long", "iovec*", "long", "long");
    info->interrupt.add_syscall(349, "kcmp", "");
    info->interrupt.add_syscall(350, "finit_module", "");

    return (1);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de connaitre le nom du plugin
**
** \return Retourne un pointeur vers le nom du plugin
*/
const char    *PLUGIN_FUNC_GET_PLUGIN_NAME()
{
    return (PLUGIN_NAME);
}

/**
** \fn int execute(Info *info)
** \brief Fonction permettant d'initialiser la liste des interruptions pour Linux
**
** \param info Classe contenant les infos du programme a analyser
** \return Retourne 1 si OK, 0 si le plugin n'a pas ete execute et -1 en cas d'erreur
*/
int    PLUGIN_FUNC_EXECUTE(Info *info)
{
    if (info == NULL)
        return (-1);

    if (1)/*(info->os == Info::OS_SYSV) || (info->os == Info::OS_LINUX))*/
    {
        if ((info->archi == Info::ARCHI_I32) || (info->archi == Info::ARCHI_I64))
            return (init_interrupt_linux_i386(info));
    }
    return (0);
}

/**
** \fn const char *get_plugin_name()
** \brief Fonction permettant de d'acceder a la description du plugin
**
** \return Retourne un pointeur vers la description du plugin
*/
const char    *PLUGIN_FUNC_APROPOS()
{
    return ("Plugin permettant d'initialiser la liste des interruptions pour Linux.\n"
            "\n"
            "Type : " PLUGIN_DESCR_TYPE "\n"
            "Priorite : " PLUGIN_DESCR_PRIORITE "\n"
            "Nom du plugin : " PLUGIN_DESCR_NAME "\n"
            "Version : " PLUGIN_DESCR_VERSION "\n"
            "\n"
            "Infos necessaires :\n"
            " - Le champ \"archi\" doit valoir \"ARCHI_I32\" ou \"ARCHI_I64\".\n"
            "\n"
            "Effets :\n"
            " - Toutes les prototypes d'interruptions sont supprimes.\n"
            " - Les prototypes des interruptions standards sont ajoutes au gestionnaire\n"
            "   d'interruptions.\n"
            "\n"
            "Valeurs de retour :\n"
            " - Retourne 1 si les interruptions ont ete initialisees.\n"
            " - Retourne 0 si les interruptions n'ont pas ete initialisees.\n"
            " - Retourne -1 en cas d'erreur.\n");
}

