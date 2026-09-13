{
  pkgs,
  lib,
  config,
  ...
}:
{
  packages = [
    pkgs.cmake
    pkgs.ninja
    pkgs.gtest
  ];


  stdenv = pkgs.clangStdenv;

  languages.cplusplus.enable = true;

  git-hooks.hooks = {
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
