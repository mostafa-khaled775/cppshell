{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        src = ./.;
      in
      {
        devShells.default = import ./shell.nix { inherit pkgs; };
        packages.default = pkgs.stdenv.mkDerivation {
          inherit system src;
          name = "lsh";
          nativeBuildInputs = [ pkgs.cmake ];
        };
      }
    );
}
