#!/bin/bash

out_dir=.lib-build

rm -r $out_dir

libtid=libtid
tobiid=tobiid
tobiic=tobiic
tobicore=tobicore

lib_dir=lib


mkdir -p $out_dir
mkdir -p $out_dir/$libtid     $out_dir/$tobiid     $out_dir/$tobiic     $out_dir/$tobicore
mkdir -p $out_dir/$tobicore/$tobicore   $out_dir/$tobiid/$tobiid
mkdir -p $out_dir/$tobiic/$tobiic       $out_dir/$libtid/$libtid

mkdir -p $out_dir/$tobicore/$lib_dir     $out_dir/$tobiid/$lib_dir
mkdir -p $out_dir/$tobiic/$lib_dir       $out_dir/$libtid/$lib_dir


cp -rp ./lib/tobicore/*  $out_dir/$tobicore/$lib_dir
cp -rp ./lib/tobiid/*    $out_dir/$tobiid/$lib_dir
cp -rp ./lib/tobiic/*    $out_dir/$tobiic/$lib_dir
cp -rp ./lib/libtid/*    $out_dir/$libtid/$lib_dir

cp -rp ../src/tobicore/*.hpp  $out_dir/$tobicore/$tobicore
cp -rp ../src/tobiid/*.hpp  $out_dir/$tobiid/$tobiid
cp -rp ../src/tobiic/*.hpp  $out_dir/$tobiic/$tobiic
cp -rp ../src/libtid/*.h  $out_dir/$libtid/$libtid


cp -rp ../license  $out_dir/$tobicore
cp -rp ../license  $out_dir/$libtid
cp -rp ../license  $out_dir/$tobiic
cp -rp ../license  $out_dir/$tobiid

cd $out_dir
zip -r ../lib/tobiid.zip    $tobiid
zip -r ../lib/tobiic.zip    $tobiic
zip -r ../lib/libtid_$1.zip    $libtid
zip -r ../lib/tobicore.zip  $tobicore
cd ..

rm -r $out_dir