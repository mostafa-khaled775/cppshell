{
  pkgs ? import <nixpkgs> { },
}:
with pkgs;
mkShell {
  packages = [
    cmake
    gnumake
    clang
    clang-tools
  ];
}
