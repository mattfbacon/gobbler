# Gobbler

KISS command-line argument parser. Just calls your functions based on argument types when it gets an argument.

## Build and Use in a Project

![Built with Nix](https://img.shields.io/badge/%E2%9D%84%EF%B8%8F-Built%20with%20Nix-blue)

If you're using CMake, integrating is easy. Add Gobbler as a subdirectory, then add it to your target's link libraries. There are targets for shared, static, and object linking strategies.

If you're not using CMake, you can still take the build artifact (in dist/Debug or dist/Release) and link with that. CMake can be invoked with:

```bash
# if you're cool
nix-shell --command "make release"
# or if you're basic
make release
```

## Example

There is an example in `example/`. It can be built with the Makefile, like the main project, but uses CMake, again like the main project.
