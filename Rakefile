require 'rake/clean'

REPO="/var/www/repo/dists/precise/universe/binary-amd64"

CLEAN.include("*.deb", "*.changes", "*.dsc", "libktomgirl_0.0.9-1.debian.tar.gz")

MAJOR_VERSION="0"
MINOR_VERSION="0"
MICRO_VERSION="9"
UBUNTU_VERSION="1"
WHOLE_VERSION=MAJOR_VERSION + "." + MINOR_VERSION + "." + MICRO_VERSION + "-" + UBUNTU_VERSION 
desc "build it"
task :default => :ui do
end

desc "show errors"
task :err do
	sh "cd src && make err"
end

desc "test"
task :test do
	sh "cd src && make test"
	#puts  WHOLE_VERSION
end

desc "build it"
task :ui do
	sh "cd src && make"
end

desc "rebuild notes dialog"
task :nd do
	sh "cd src && rm -f ui_notesdialog.h && make"
end

desc "Build the project (default)"
task :build  do
	puts "default"
end

desc "Upload ppa to ubuntu"
task :ppa  do
	#puts "dput ppa:follinge/precise-kde4-unasked-ppa libktomgirl_0.0.9-1_amd64.changes"
end

desc "Put in a local repository"
task :repo  do
	sh "cp *.deb #{REPO}"
	sh "dpkg-scanpackages /dev/null #{REPO} | gzip -9c > #{REPO}/Packages"
end

desc "Recreate original source package (with no debian dir)"
task :orig  do
  sh "rm -f libktomgirl_0.0.9.orig.tar.gz"
	sh "tar --exclude libktomgirl-0.0.9/debian -czvf libktomgirl_0.0.9.orig.tar.gz libktomgirl-0.0.9"
end

desc "file of dependencies"
task :deps do
	sh "ldd src/build/ktomgirl > /tmp/ldd"
	File.open("/tmp/ldd") do |f|
		while line = f.gets
			a = line.split("=>")
			next if a.length() < 2
		  b=a[1].split("(")
			next if "" == b[0].strip()
			sh "dpkg -S #{b[0]} >> /tmp/deps"
		  sh "cat /tmp/deps  | cut -d: -f1 | sort | uniq > src/dependencies"
	  end
	end
end

# Sun Oct 27 16:20:25 PDT 2013
