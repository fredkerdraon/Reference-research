#!/bin/sh
# 
# ------------------------------------------------------------
# Company: Software- und Organisations-Service GmbH
# Author : Oliver Haufe <oliver.haufe@sos-berlin.com>
# Dated  : 2015-04-13
# Purpose: starts installer 
# ------------------------------------------------------------


if [ "$1" = "-h" ] || [ "$1" = "--help" ]
then
  echo "`basename $0` [OPTION]"
  echo "options:"
  echo "  -u, --unprivileged     installer does not ask for root privileges"
  echo ""
  exit 0
fi

JAVABINPATH=

if [ "$USER" = "root" ]
then
  echo "Please don't call this script as root"
  exit 1
fi

USE_UNPRIVILEGED="n"
if [ "$1" = "-u" ] || [ "$1" = "--unprivileged" ]
then
  shift
  USE_UNPRIVILEGED="y"
fi

if [ ! -z "$1" ]
then
  CHECK_PARAM_XML=`grep "\[:choose " "$1"`
  if [ "$CHECK_PARAM_XML" != "" ]
  then
    echo "Please edit at first the file: $1"
    echo "Look into the scheduler_installation_en.pdf or more information"
    exit 1
  fi
fi

which_return="`java -version 2>&1`"
last_exit=$?

if [ $last_exit -ne 0 ]
then
  # echo "$which_return"
  echo "java couldn't be found."
  echo "Please enter your java bin directory ([java install path]/bin):"
  read JAVABIN
  which_return="`$JAVABIN/java -version 2>&1`"
  last_exit=$?
  if [ $last_exit -ne 0 ]
  then
    echo "\"$JAVABIN/java\" couldn't be found."
    exit 1
  fi
fi

if [ "$JAVABIN" != "" ]
then
  JAVABINPATH="${JAVABIN}/"
fi

if [ ! -z "$1" ]
then
  JAVA_VERSION=`"${JAVABINPATH}java" -version 2>&1 | awk 'NR==1 {print $3}' | sed -e 's/[^0-9]/ /g' | awk '{print ($1*10000)+$2}'`
  if [ ! -z "$JAVA_VERSION" ] && [ "$JAVA_VERSION" -lt 10008 ]
  then
    echo "...the java version of \"${JAVABINPATH}java\" is less than 1.8.x"
    echo "...please install a java runtime environment at least 1.8.x" 
    exit 0
  fi
fi


if [ "$USE_UNPRIVILEGED" = "y" ]
then
  echo "${JAVABINPATH}java -Dizpack.mode=privileged -jar \"`dirname $0`/jobscheduler_linux-x64.1.10.3.jar\" $*"
  ${JAVABINPATH}java -Dizpack.mode=privileged -jar "`dirname $0`/jobscheduler_linux-x64.1.10.3.jar" $*
  exit 0
fi

  
which_return="`which sudo 2>&1`"
sudo_exit=$?
USESU=n 

export DISPLAY
if [ -f "$HOME/.Xauthority" ]
then 
  XAUTHORITY="$HOME/.Xauthority"
  export XAUTHORITY
fi

my_sudo() {
  #Sudo option -E supported since 1.6.9
  sudo_option="-E"
  sudo_version=`sudo -V | head -1 | sed -e 's/.*\([1-9]\{1,\}\)\.\([0-9]\{1,\}\)\.\([0-9]\{1,\}\).*/\1\2\3/'`
  sudo_version_is_number=`echo "$sudo_version" | sed -e 's/[0-9]//g'`
  if [ -z "$sudo_version_is_number" ]
  then
    if [ "$sudo_version" -lt "169" ]
    then
      #Sudo version less than 1.6.9
      sudo_option=""
    fi
  else
    #Sudo version not found
    sudo_option=""
  fi
  echo "sudo $sudo_option $*"
  sudo $sudo_option $*
  sudo_exit=$?
}

if [ $sudo_exit -eq 0 ]
then
  my_sudo ${JAVABINPATH}java -jar "`dirname $0`/jobscheduler_linux-x64.1.10.3.jar" $*
  if [ $sudo_exit -ne 0 ]
  then
    echo "Do you want to use 'su' instead of 'sudo'? (y or n)"
    read USESU
  fi
else
  USESU=y 
fi

if [ "$USESU" = "y" ] || [ "$USESU" = "Y" ]
then
  echo "su root -c \"${JAVABINPATH}java -jar \\\"`dirname $0`/jobscheduler_linux-x64.1.10.3.jar\\\" $*\""
  su root -c "${JAVABINPATH}java -jar \"`dirname $0`/jobscheduler_linux-x64.1.10.3.jar\" $*"
fi