import struct

def bytes_to_float(string_of_bytes):
    if len(string_of_bytes)<4:  #if there are not 4 bytes then it cannot be translated into a float and therefore an error is obtained
        return (False,25.0)
    else:
        tuplet_float= struct.unpack_from('f',string_of_bytes)   #uncompress string of 4 bytes into a floting number
        return (True,tuplet_float[0])

