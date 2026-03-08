require_relative "support/build.rb"

include_path("support")
include_path("interface")
include_path("source")

task :clobber do
    FileUtils.rm_rf(ENV["BUILD_DIR"])
end

task :build do
    Dir.new("source").files(-1, ".c").each do |c|
        compile(c)
    end
end

task :test => :build do
    Dir.new("support/simulation").files(-1, ".c").each do |c|
        compile(c)
    end

    Dir.new("test").files(-1, ".c").each do |t|
        exe = program(t)
        cmd([exe], "Run #{File.basename(t)}")
    end
end
