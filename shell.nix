with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "metroid";
  buildInputs = [avrdude avrgcclibc python3 python3Packages.scikitimage python3Packages.numpy];
}

