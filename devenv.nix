{
  pkgs,
  ...
}:
{
  packages = [
    pkgs.cmake
    pkgs.ninja
    pkgs.gtest
    pkgs.llvmPackages.openmp
    pkgs.openmpi
  ];


  stdenv = pkgs.clangStdenv;

  languages.cplusplus.enable = true;

  git-hooks.hooks = {
    aaa-cmake-configure = {
      enable = true;
      name = "cmake configure";
      entry = "cmake -S . -B build -G Ninja";
      language = "system";
      pass_filenames = false;
      types_or = [
        "c"
        "c++"
      ];
    };
    clang-format = {
      enable = true;
      types_or = [
        "c"
        "c++"
      ];
    };
    clang-tidy = {
      enable = true;
      types_or = [
        "c"
        "c++"
      ];
    };
  };

  enterShell = ''
    clang --version
    cmake --version
    ninja --version
  '';

}
