# nodetach
`nodetach` is a simple program that prevents a subprocess from detaching itself

# Usage
```
$ ./nodetach my-server --flags --go --here
```

# Internals
`nodetach` works by calling [prctl(2)] with the `PR_SET_CHILD_SUBREAPER` flag to
make itself a process reaper. This has the effect that when a process dies which
was indirectly started by `nodetach` it will get re-parented to `nodetach`
instead of to `init`.

Once doing that, `nodetach` simply starts a subprocess using `posix_spawn(3)`

[prctl(2)]: http://man7.org/linux/man-pages/man2/prctl.2.html
[posix_spawn(3)]: http://man7.org/linux/man-pages/man3/posix_spawn.3.html
