let 
  fetch    = (import <nixpkgs> {}).fetchFromGitHub;
  pinned   = fetch {
    owner  = "NixOS";
    repo   = "nixpkgs";
    rev    = "56da88a298a6f549701a10bb12072804a1ebfbd5"; # NixOS 17.03
    sha256 = "0cacw30vy4xswpkj3vbw92xfv5q06mw22msq0i54gphmw2r5iizh";
  };
  pkgs     = import pinned {};
  midi     = pkgs.python2Packages.buildPythonPackage rec {
    name = "midi-${version}";
    version = "0.2.3";
    src = fetch {
      owner  = "vishnubob";
      repo   = "python-midi";
      rev    = "4b7a229f6b340e7424c1fccafa9ac543b9b3d605";
      sha256 = "1pldwrz3yfavjlc9n0frvdfyqnmn3rcgy701pc4424lmqp18c6zj";
    };
  };
in pkgs.stdenv.mkDerivation {
  name = "metroid";
  buildInputs = with pkgs; [
    avrdude avrgcclibc
    (python3.withPackages (ps: with ps; [numpy scikitimage])) # for sprites
    (python2.withPackages (ps: with ps; [midi]))              # for sound
  ];
}

