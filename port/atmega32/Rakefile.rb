

def run(cmd)
    system(*(cmd.flatten))
end

desc "Build the atmega32 port"
task :build do
    FileUtils.mkdir_p("build")

    includes = Dir["../../source/**/"]
    files = [
        "main.c",
        "../../source/shell/shell.c",
        "../../source/shell/shell_time.c",
        "../../source/os/os_time.c",
    ] + Dir["../../source/util/*.c"]

    objects = files.map {|f| File.join("build", File.basename(f).sub(".c", ".o"))}

    files.each_with_index do |c, i|
        cmd = [
            "avr-gcc",
            "-Wall",
            "-Os",
            "-mmcu=atmega32",
            includes.map {|i| "-I#{i}"},
            "-c", c,
            "-o", objects[i]
        ]
        run(cmd)
    end

    run(["avr-gcc", "-mmcu=atmega32", objects, "-o", "build/main.elf"])
    run(%W[avr-objcopy -O ihex -R .eeprom build/main.elf build/main.hex])
end

task :flash => [:build] do
    run(%W[avrdude -c jtag3 -p m32 -U flash:w:build/main.hex:i])
end

task :fuse do
    run(%W[avrdude -c jtag3 -p m32 -U lfuse:w:0xFF:m -U hfuse:w:0x99:m])
end
