Import("env")

platform = env.PioPlatform()

boot_app_bin = platform.get_package_dir("framework-arduinoespressif32") + '/tools/partitions/boot_app0.bin'

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.bin", 
    "esptool.py --chip esp32s3 merge_bin -o $BUILD_DIR/${PROGNAME_RAW}_FLASH.bin  " + 
    " --flash_mode dio --flash_freq 80m --flash_size 16MB " +
    " 0x0000 $BUILD_DIR/bootloader.bin " +
    " 0x8000 $BUILD_DIR/partitions.bin " + 
    " 0xe000 " + boot_app_bin + 
    " 0x10000 $BUILD_DIR/${PROGNAME}.bin"
)
