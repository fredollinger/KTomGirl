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
end

desc "build it"
task :ui do
	sh "cd src && make"
end

desc "rebuild notes dialog"
task :nd do
	sh "cd src && rm -f ui_notesdialog.h && make"
end

