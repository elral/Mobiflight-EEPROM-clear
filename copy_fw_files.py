Import("env")
import os, zipfile, shutil
from pathlib import Path

firmware_version = "1.0.2"

zip_file_name = 'Mobiflight-Connector'
build_path = './_build'
build_path_fw = build_path + '/firmware'
build_path_json = build_path + '/Boards'
distrubution_path = './_dist'
board_folder = ['./_Boards/Atmel', './_Boards/RaspberryPi', './_Boards/ESP32']
platform = env.BoardConfig().get("platform", {})

def copy_fw_files (source, target, env):
    fw_file_name=str(target[0])

    if platform == "espressif32":
        merge_bin(source, target, env)
        old_name = fw_file_name[0:-4] + "_merged.bin"
        fw_file_name = fw_file_name[0:-9]  + "merged_" + firmware_version.replace(".", "_") + ".bin"
        os.replace(old_name, fw_file_name)


# Script from: https://github.com/platformio/platform-espressif32/issues/1078
# creates one .bin file for ESP32 which can be flashed at once
# e.g. C:/Users/ralfk/.platformio/penv/Scripts/python.exe C:/Users/ralfk/.platformio/packages/tool-esptoolpy/esptool.py write_flash 0x0000 C:/_PlatformIO/MobiFlight-FirmwareSource/.pio/build/mobiflight_esp32_s2_mini/mobiflight_esp32_s2_mini_merged_0_0_1.bin
APP_BIN = "$BUILD_DIR/${PROGNAME}.bin"
MERGED_BIN = "$BUILD_DIR/${PROGNAME}_merged.bin"
BOARD_CONFIG = env.BoardConfig()

def merge_bin(source, target, env):
    # The list contains all extra images (bootloader, partitions, eboot) and
    # the final application binary
    flash_images = env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])) + ["$ESP32_APP_OFFSET", APP_BIN]

    # Run esptool to merge images into a single binary
    env.Execute(
        " ".join(
            [
                "$PYTHONEXE",
                "$OBJCOPY",
                "--chip",
                BOARD_CONFIG.get("build.mcu", "esp32"),
                "merge_bin",
                "--fill-flash-size",
                BOARD_CONFIG.get("upload.flash_size", "4MB"),
                "-o",
                MERGED_BIN,
            ]
            + flash_images
        )
    )


# env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", copy_fw_files)
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_fw_files)
