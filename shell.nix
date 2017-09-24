let 
  fetch      = (import <nixpkgs> {}).fetchFromGitHub;
  pinned     = fetch {
    owner    = "NixOS";
    repo     = "nixpkgs";
    rev      = "56da88a298a6f549701a10bb12072804a1ebfbd5"; # NixOS 17.03
    sha256   = "0cacw30vy4xswpkj3vbw92xfv5q06mw22msq0i54gphmw2r5iizh";
  };
  pkgs       = import pinned {};
  mido       = pkgs.python2Packages.buildPythonPackage rec {
    name     = "mido-${version}";
    version  = "1.2.8";
    src      = pkgs.fetchurl {
      url    = "mirror://pypi/m/mido/mido-${version}.tar.gz";
      sha256 = "0griab8f54d3phf07p99s750xjw7vx2sdhydxgyca7ajsis2h51m";
    };
  };
in pkgs.stdenv.mkDerivation {
  name = "metro-hd";
  buildInputs = with pkgs; [
    avrdude avrgcclibc
    (python3.withPackages (ps: with ps; [
      numpy scikitimage                          # for sprites
      pelican markdown                           # for website
    ]))
    (python2.withPackages (ps: with ps; [mido])) # for sound
  ];
}

