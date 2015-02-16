import struct

def parse_utf8(self, bytes, length_size):

    length = bytes2int(bytes[0:length_size])
    value = ''.join(['%c' % b for b in bytes[length_size:length_size+length]])
    return value


def bytes2int(raw_bytes, signed=False):
    """
    Convert a string of bytes to an integer (assumes little-endian byte order)
    """
    if len(raw_bytes) == 0:
        return None
    fmt = {1:'B', 2:'H', 4:'I', 8:'Q'}[len(raw_bytes)]
    if signed:
        fmt = fmt.lower()
    #return struct.unpack('<'+fmt, raw_bytes)[0]
    tuplet_int = struct.unpack('<'+fmt, raw_bytes)[0]
    return (True,tuplet_int[0])

def bytes_to_int(string_of_bytes):
    if len(string_of_bytes)<4:  #if there are not 4 bytes then it cannot be translated into a float and therefore an error is obtained
        print  "byte_to_int: there are less than four bytes "        
        return (False,25.0)
    else:
        tuplet_int= struct.unpack_from('i',string_of_bytes)   #uncompress string of 4 bytes into a floting number
        #print "byte_to_float: " + str(tuplet_float[0])
        return (True,tuplet_int[0])


def bytes_to_string(string_of_bytes):
    if len(string_of_bytes)<4:  #if there are not 4 bytes then it cannot be translated into a float and therefore an error is obtained
        print  "byte_to_int: there are less than four bytes "        
        return (False,25.0)
    else:
        tuplet_int= struct.unpack_from('s',string_of_bytes)   #uncompress string of 4 bytes into a floting number
        #print "bytes_to_string: " + str(tuplet_int[0])
        return (True,tuplet_int[0])
