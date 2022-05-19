// ignore_for_file: camel_case_types

import 'dart:ffi';
import 'dart:io' show Directory;
import 'package:path/path.dart' as path;
import 'package:ffi/ffi.dart';

typedef nativeFunc = Int32 Function(Int32 a, Int32 b);
typedef native = int Function(int a, int b);

typedef helloFunc = Pointer<Utf8> Function();
typedef hello = Pointer<Utf8> Function();

void main(List<String> args) {
  final libnativePath =
      path.join(Directory.current.path, 'lib', 'libnative.so');

  final libnative = DynamicLibrary.open(libnativePath);

  final nativ =
      libnative.lookup<NativeFunction<nativeFunc>>('sum').asFunction<native>();

  final hellovar =
      libnative.lookup<NativeFunction<helloFunc>>('hello').asFunction<hello>();

  print(hellovar().toDartString());
}
