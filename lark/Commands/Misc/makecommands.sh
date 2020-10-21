#! /usr/bin/bash

if [[ ! -f ./lark.csproj]]; then
  echo Can only run this script in the project root directory where the .csproj file is.
  exit 1
fi

