require 'formula'

class BsdgamesOsx < Formula
  homepage 'https://github.com/ctdk/bsdgames-osx'
  url 'https://github.com/ctdk/bsdgames-osx/archive/bsdgames-osx-2.19.3.tar.gz'
  sha1 '31013cbc8fbad71f1e3e0b9b85fd7c943219a99b'
  head 'https://github.com/ctdk/bsdgames-osx.git'
  version '2.19.3'
  depends_on :bsdmake => :build

  def install
    ENV.j1
    # This replicates the behavior of wargames calling games from /usr/games
    inreplace 'wargames/wargames.sh' do |s|
      s.gsub! /\/usr\/games/, "#{prefix}/bin"
    end
    system "bsdmake PREFIX=#{prefix} VARDIR=#{HOMEBREW_PREFIX}/var/games"
    system "bsdmake install PREFIX=#{prefix} VARDIR=#{HOMEBREW_PREFIX}/var/games"
  end

  def test
    %w[ pom ].each do |game|
  system game
      end
  end
end
