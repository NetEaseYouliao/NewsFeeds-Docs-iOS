#!/bin/bash

currentDir=$(cd `dirname $0`; pwd)
cd $currentDir


appname="NewsFeedsUIDemo"
appnewname="Angel"
configuration="Debug"
# configuration="Release"

path="$HOME/Desktop/auto_archive"
echo $path
mkdir $path



logInfo=$(git log -1)

commitTag=${logInfo##*commit }
commitTag=${commitTag%%Author*}

strLength=${#commitTag}
if (($strLength > 0))
then
commitTag=${commitTag:0:strLength-1}
else
commitTag=${logInfo:0:20}
fi

echo "$commitTag"



releaseDir=$path/Build/Products/$configuration-iphoneos
echo $releaseDir

xcodebuild clean -project $appname.xcodeproj -configuration $configuration -alltargets
xcodebuild -workspace $appname.xcworkspace -scheme $appname archive -archivePath $path/$appnewname.xcarchive


send=`date '+%Y%m%d%H%M'`
xcodebuild -exportArchive -archivePath $path/$appnewname.xcarchive -exportPath $path/$appnewname.ipa -exportOptionsPlist ./NewsFeedsUIDemo/IPA.plist



mv $path/$appname.xcarchive $path/NewsFeedsDemo_${send}_$commitTag.xcarchive

open $path
