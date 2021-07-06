#!/bin/sh

vendor/premake/premake5 gmake2 && make -C build config=release && cp build/onvd/bin/Release/onvd .