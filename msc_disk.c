#include "tusb.h"
#include <string.h>
#include <stdint.h>

// 16KB disk (32 sectors of 512 bytes)
#define DISK_BLOCK_NUM  128
#define DISK_BLOCK_SIZE 512

// Preformatted FAT12 image with PicoS2 volume label and two .TXT files
extern uint8_t msc_disk[DISK_BLOCK_NUM][DISK_BLOCK_SIZE] = {
    #include "pico_s2_disk_image.inc"
};

// MSC Callbacks
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    memcpy(vendor_id,  "PicoS2  ", 8);
    memcpy(product_id, "ConfigDisk     ", 16);
    memcpy(product_rev, "1.0", 4);
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) { return true; }
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    *block_count = DISK_BLOCK_NUM;
    *block_size  = DISK_BLOCK_SIZE;
}
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    if (lba >= DISK_BLOCK_NUM) return -1;
    memcpy(buffer, msc_disk[lba] + offset, bufsize);
    return bufsize;
}
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    if (lba >= DISK_BLOCK_NUM) return -1;
    memcpy(msc_disk[lba] + offset, buffer, bufsize);
    return bufsize;
}
void tud_msc_write10_complete_cb(uint8_t lun) { }
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) { return true; }

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) {
    // Return -1 for unsupported SCSI commands
    (void)lun;
    (void)scsi_cmd;
    (void)buffer;
    (void)bufsize;
    return -1;
}