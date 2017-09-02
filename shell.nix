let 
  fetch    = (import <nixpkgs> {}).fetchFromGitHub;
  pinned   = fetch {
    owner  = "NixOS";
    repo   = "nixpkgs";
    rev    = "56da88a298a6f549701a10bb12072804a1ebfbd5"; # NixOS 17.03
    sha256 = "0cacw30vy4xswpkj3vbw92xfv5q06mw22msq0i54gphmw2r5iizh";
  };
  pkgs     = import pinned {};
in pkgs.stdenv.mkDerivation {
  name = "metroid";
  buildInputs = with pkgs; [
    avrdude avrgcclibc
    (python3.withPackages (ps: with ps; [numpy scikitimage]))
    python2
  ];
}

