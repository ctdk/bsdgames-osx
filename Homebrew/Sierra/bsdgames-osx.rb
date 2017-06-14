require 'formula'

class BsdgamesOsx < Formula
  homepage 'https://github.com/ctdk/bsdgames-osx'
  url 'https://github.com/ctdk/bsdgames-osx/archive/bsdgames-osx-2.19.3.tar.gz'
  sha256 '699bb294f2c431b9729320f73c7fcd9dcf4226216c15137bb81f7da157bed2a9'
  head 'https://github.com/ctdk/bsdgames-osx.git'
  version '2.19.3'
  depends_on "bsdmake" => :build

  def install
    ENV.deparallelize
    # This replicates the behavior of wargames calling games from /usr/games
    inreplace 'wargames/wargames.sh' do |s|
      s.gsub! /\/usr\/games/, "#{prefix}/bin"
    end
    system "CFLAGS=\"-std=c11 -Wno-nullability-completeness\" bsdmake PREFIX=#{prefix} VARDIR=#{HOMEBREW_PREFIX}/var/games"
    user = ENV['USER']
    system "BINOWN=#{user} LIBOWN=#{user} MANOWN=#{user} SHAREOWN=#{user} bsdmake install PREFIX=#{prefix} VARDIR=#{HOMEBREW_PREFIX}/var/games"
  end

  def test
    %w[ pom ].each do |game|
  system game
      end
  end
end

