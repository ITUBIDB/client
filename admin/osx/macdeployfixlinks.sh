#/bin/bash

if [[ "${#}" -eq 1 && "${1}" =~ .app$ ]]
then
    appRoot="${1}"
else
    echo "Usage: ${0} <path-to-app.app>"
    exit 1
fi

for binary in $(find "${appRoot}" -type f -a \( -name '*.dylib' -o \! -name '*.*' \))
do
    if file "${binary}" | grep -Eq 'Mach-O 64-bit executable x86_64|Mach-O 64-bit dynamically linked shared library x86_64'
    then
        echo "Checking ${binary}"
        for line in $(otool -L "${binary}")
        do
            #if line is name of the file
            if echo "${line}" | grep -Fxq "${binary}:"
            then
                continue
            fi

            #get library name
            libFullName=$(echo "${line}" | awk '{print$1}')
            libBaseName=$(basename "${libFullName}")

            #if library link is full path
            if [[ "${libFullName}" =~ ^/ ]]
            then
                #search app folder for library
                linkAppPath=$(find "${appRoot}" -name "${libBaseName}" | head -n 1)

                #if found in app, fix it
                if [[ "${linkAppPath}" != "" ]]
                then
                    #prepare link path
                    linkAppPath=$(echo "${linkAppPath}" | sed "s#${appRoot}/Contents/#@executable_path/../#")

                    #apply change
                    if install_name_tool -change "${libFullName}" "${linkAppPath}" -id "${libBaseName}" "${binary}"
                    then
                        echo "Fixed: ${libBaseName}"
                    else
                        echo "Failed: ${libBaseName}"
                    fi
                fi
            fi
        done
    fi
done
