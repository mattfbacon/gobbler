{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	nativeBuildInputs = with pkgs; [ clang lldb catch2 ccls ];
	buildInputs = with pkgs; [ gcc python3 gnumake cmake lld ];
}
