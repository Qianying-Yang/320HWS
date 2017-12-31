#include "utf.h"
#include "debug.h"
#include "wrappers.h"
#include <unistd.h>

int from_utf16le_to_utf16be(int infile, int outfile)
{
  printf("%s\n", "HERE IN ENCODEING");
  int bom;
  utf16_glyph_t buf;
  ssize_t bytes_read;
  size_t bytes_to_write;
  int ret = 0;

  bom = UTF16BE;
//#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  reverse_bytes(&bom, 2);
//#endif
  //write_to_bigendian(outfile, &bom, 2);

  while ((bytes_read = read_to_bigendian(infile, &(buf.upper_bytes), 2)) > 0) {
    bytes_to_write = 2;
    reverse_bytes(&(buf.upper_bytes), 2);
    if(is_lower_surrogate_pair(buf)) {
      if((bytes_read = read_to_bigendian(infile, &(buf.lower_bytes), 2)) < 0) {
        break;
      }
      reverse_bytes(&(buf.lower_bytes), 2);
      bytes_to_write += 2;
    }
    write_to_bigendian(outfile, &buf, bytes_to_write);
  }
  ret = bytes_read;
  return ret;
}

int from_utf16le_to_utf8(int infile, int outfile)
{
  int ret = 0;
  code_point_t code_point;
  //convert to UTF8
  int bom = UTF8;
  int count = 0;
  //size_t remaining_bytes;
  size_t size_of_glyph = 1;
  size_t bytes_read;
  utf16_glyph_t utf16_buf;
  utf8_glyph_t utf8_buf;
  utf16_glyph_t temp;
  //temp.upper_bytes = 0x0000;

  reverse_bytes(&bom, 3);
  write_to_bigendian(outfile, &bom, 3);

  while((bytes_read = read_to_bigendian(infile, &utf16_buf, 2)) > 0) {

    if(is_upper_surrogate_pair(utf16_buf)) {
      temp.upper_bytes = utf16_buf.upper_bytes;
      bytes_read = read_to_bigendian(infile, &utf16_buf, 2);
      temp.lower_bytes = utf16_buf.upper_bytes;
      code_point = utf16_glyph_to_code_point(&temp);
      size_of_glyph = 5;
      utf8_buf = code_point_to_utf8_glyph(code_point, &size_of_glyph);
    }

    else {
      code_point = utf16_glyph_to_code_point(&utf16_buf);
      utf8_buf = code_point_to_utf8_glyph(code_point, &size_of_glyph);
    }

    if(count > 0) {
      write_to_bigendian(outfile, &utf8_buf, size_of_glyph);
    }
    count++;
  }

  ret = bytes_read;
  return ret;
}

utf16_glyph_t code_point_to_utf16le_glyph(code_point_t code_point, size_t *size_of_glyph)
{
  utf16_glyph_t ret;
  memeset(&ret, 0, sizeof ret);

  if(is_code_point_surrogate(code_point)) {
    //temp = code_poiny - 0x10000;
    code_point_t temp = code_point - 0x10000;

    //ten_msbs = code_point & 0xFFC0;
    code_point_t ten_msbs = code_point & 0xFFC0;
    code_point_t ten_lsbs = temp & 0x03FF;

    //ten_msbs = ten_msbs >> 10
    ten_msbs = ten_msbs >> 10;

    //changed to >> 10. begine with [<< 100]
    ret.upper_bytes = ten_msbs + 0xD800;
    ret.lower_bytes = ten_lsbs + 0xDC00;

  #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    reverse_bytes(&ret.upper_bytes, 2);
    ret.upper_bytes = ret.upper_bytes << 16;
    reverse_bytes(&ret.lower_bytes, 2);
  #endif
    *size_of_glyph = 4;
  }
  else {
    ret.upper_bytes |= code_point;
  #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    reverse_bytes(&ret.upper_bytes, 2);
  #endif
    *size_of_glyph = 2;
  }
  return ret;
}
