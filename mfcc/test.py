
with open("/home/adrianords/Documentos/mfcc_dart_native/mfcc/buffer2", "r") as a:
    b = a.readline().replace(',', ' ')
    with open("/home/adrianords/Documentos/mfcc_dart_native/mfcc/buffer2_esp", "w") as c:
        c.write(b)
