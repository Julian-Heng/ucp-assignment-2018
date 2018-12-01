#!/usr/bin/env bash

err()
{
    printf "%s\\n" "Error: $*" >&2
}

add()
{
    awk_script='
        BEGIN {
            printf "%0.6f", a + b
        }'

    [[ "$1" && "$2" ]] && \
        awk -v a="$1" -v b="$2" "${awk_script}"
}

minus()
{
    awk_script='
        BEGIN {
            printf "%0.6f", a - b
        }'

    [[ "$1" && "$2" ]] && \
        awk -v a="$1" -v b="$2" "${awk_script}"
}

double_bound()
{
    awk_script='
        BEGIN {
            if (num > low && up > num)
                code = 0
            else
                code = 1

            exit code
        }'

    if [[ "$1" && "$2" && "$3" ]] && \
       awk -v num="$1" -v low="$2" -v up="$3" "${awk_script}"; then
        return 0
    else
        return 1
    fi
}

int_bound()
{
    if [[ "$1" && "$2" && "$3" ]] && (($1 >= $2 && $3 >= $1)); then
        return 0
    else
        return 1
    fi
}

words()
{
    words=($*)
    printf "%s" "${#words[@]}"
}

print_file_error()
{
    err "$1: Invalid command on line $2: $3"
    file_valid="false"
    exit_status="true"
}

validate_files()
{
    local i="0"
    local j="0"
    local valid="true"
    local file_valid="true"
    local exit_status
    while ((i < ${#file_list[@]})) && [[ "${valid}" == "true" ]]; do
        file_valid="true"
        mapfile -t file < "${file_list[$i]}"
        while ((j < ${#file[@]})) && [[ "${file_valid}" == true ]]; do
            if (($(words "${file[$j]}") == 2)) || \
               (($(words "${file[$j]}") == 0)); then
                read -r cmd arg <<< "${file[$j]}"
                case "${cmd,,}" in
                    "rotate")
                        ! double_bound "${arg}" "-360.0" "360.0" && \
                            print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;

                    "move"|"draw")
                        ! ((${arg/.*} >= 0)) && \
                            print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;

                    "fg")
                        ! int_bound "${arg}" "0" "15" && \
                            print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;

                    "bg")
                        ! int_bound "${arg}" "0" "7" && \
                            print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;

                    "pattern")
                        ! { ((${#arg} == 1)) && [[ "${arg}" != " " ]]; } && \
                            print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;

                    *)
                        print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
                    ;;
                esac
            else
                print_file_error "${file_list[$i]}" "$((j + 1))" "${file[$j]}"
            fi
            ((j++))
        done
        ((i++))
    done

    if [[ "${exit_status}" == "true" ]]; then
        return 1
    else
        return 0
    fi
}

draw()
{
    local x1 y1 x2 y2 x3 y3 x4 y4 x5 y5
    local angle pat
    local fg bg
    local execute data
    local isZero

    x1="0.0"
    y1="0.0"

    x2="0.0"
    y2="0.0"

    x3="0"
    y3="0"

    x4="0"
    y4="0"

    x5="0.0"
    y5="0.0"

    fg="7"
    bg="0"

    setFgColor "${fg}"
    setBgColor "${bg}"

    for file in "${file_list[@]}"; do
        clearScreen
        mapfile -t file_contents < "${file}"

        for line in "${file_contents[@]}"; do
            isZero="false"
            x3="0"
            y3="0"

            x4="0"
            y4="0"

            x5="0.0"
            y5="0.0"

            execute=""
            data=""

            read -r cmd arg <<< "${line}"

            case "${cmd,,}" in
                "rotate")
                    angle="$(add "${angle:-0}" "${arg}")"
                ;;

                "move")
                    if ((${arg/.*} == 0 && ${arg/*.} == 0)); then
                        isZero="true"
                    else
                        calcNewPosition "${angle:-0}" "$(minus "${arg}" "-1.0")"
                        execute="doNothing"
                        data=""
                    fi
                ;;

                "draw")
                    if ((${arg/.*} == 0 && ${arg/*.} == 0)); then
                        isZero="true"
                    else
                        calcNewPosition "${angle:-0}" "$(minus "${arg}" "-1.0")"
                        execute="putChar"
                        data="${pat}"
                    fi
                ;;

                "fg")
                    fg="${arg}"
                    setFgColor "${fg}"
                ;;

                "bg")
                    bg="${arg}"
                    setBgColor "${bg}"
                ;;

                "pattern")
                    pat="${arg}"
                ;;
            esac

            [[ "${execute}" && "${isZero}" != "true" ]] && {
                printf -v x3 "%.*f" "0" "${x1}"
                printf -v y3 "%.*f" "0" "${y1}"

                printf -v x4 "%.*f" "0" "${x2}"
                printf -v y4 "%.*f" "0" "${y2}"

                x5="${x1}"
                y5="${y1}"

                calcNewPosition "${angle}" "1.0"

                printf -v x1 "%.*f" "0" "${x1}"
                printf -v y1 "%.*f" "0" "${y1}"

                printf -v x2 "%.*f" "0" "${x2}"
                printf -v y2 "%.*f" "0" "${y2}"

                line "${x3}" "${y3}" "${x4}" "${y4}" "${execute}" "${data}"
            }
        done

        printf "\033[0mj"
        penDown
    done
}

calcNewPosition()
{
    local xDelta yDelta
    local angle="$1"
    local distance="$2"

    awk_cos='
        BEGIN {
            printf "%0.6f", b * cos(a * (atan2(0, -1) / 180))
        }'

    awk_sine='
        BEGIN {
            printf "%0.6f", b * sin(a * (atan2(0, -1) / 180))
        }'

    xDelta="$(awk -v a="${angle}" -v b="${distance}" "${awk_cos}")"
    yDelta="$(awk -v a="${angle}" -v b="${distance}" "${awk_sine}")"

    x1="${x2}"
    x2="$(add "${x2}" "${xDelta}")"

    y1="${y2}"
    y2="$(minus "${y2}" "${yDelta}")"
}

line()
{
    (($# < 6)) && \
        return 1

    local x1="$1"
    local y1="$2"
    local x2="$3"
    local y2="$4"
    local execute="$5"
    local char="$6"

    local x="${x1}"
    local y="${y1}"

    ((majorDelta = x2 - x1))
    majorMove="right"

    ((majorDelta < 0)) && {
        ((majorDelta = -majorDelta))
        majorMove="left"
    }

    ((minorDelta = y2 - y1))
    minorMove="down"

    ((minorDelta < 0)) && {
        ((minorDelta = -minorDelta))
        minorMove="up"
    }

    ((minorDelta > majorDelta)) && {
        tempDelta="${majorDelta}"
        tempMove="${majorMove}"

        majorDelta="${minorDelta}"
        majorMove="${majorMove}"

        minorDelta="${tempDelta}"
        minorMove="${tempMove}"
    }

    ((decision = majorDelta / 2))

    for ((i = 0; i <= majorDelta; i++)); do
        printf "\\033[%d;%dH" "$((y + 1))" "$((x + 1))"

        case "${execute}" in
            "doNothing") doNothing ;;
            "putChar") putChar "${char}" ;;
        esac

        case "${majorMove}" in
            "right") ((x++)) ;;
            "left") ((x--)) ;;
            "up") ((y--)) ;;
            "down") ((y++)) ;;
        esac

        ((decision += minorDelta))

        ((decision >= majorDelta)) && {
            ((decision -= majorDelta))

            case "${minorMove}" in
                "right") ((x++)) ;;
                "left") ((x--)) ;;
                "up") ((y--)) ;;
                "down") ((y++)) ;;
            esac
        }
    done
}

clearScreen()
{
    printf "\\033[2J"
}

penDown()
{
    printf "\\033[10000;1H"
}

setFgColor()
{
    printf "\\033[22;%dm" "$((($1 % 8) + 30))"
    ((($1 % 16) >= 8)) && printf "\\033[1m"
}

setBgColor()
{
    printf "\\033[%dm" "$((($1 % 8) + 40))"
}

doNothing()
{
    :
}

putChar()
{
    printf "%c" "$1"
}

print_help()
{
    printf "%s\\n" "Usage: turtle [FILE]
Draw a graphic from commands in FILE
Example: turtle ./picture.txt

Valid commands:

    +---------+-------+-----------------------+
    | Command | Type  | Range                 |
    +---------+-------+-----------------------+
    | ROTATE  | float | -360 to 360 inclusive |
    | MOVE    | float | Positive              |
    | DRAW    | float | Positive              |
    | FG      | int   | 0 to 15 inclusive     |
    | BG      | int   | 0 to 7 inclusive      |
    | PATTERN | char  | Any character         |
    +---------+-------+-----------------------+
"
}

print_version()
{
    printf "%s\\n" "turtle: A terminal drawing program
Written by Julian Heng
"
}

check_args()
{
    (($# == 0)) && {
        print_help
        exit 1
    }

    while (($# > 0)); do
        case "$1" in
            "-d"|"--debug") debug="true" ;;
            "-h"|"--help") print_help; exit 0 ;;
            "--version") print_version; exit 0 ;;
            *) [[ -f "$1" ]] && file_list+=("$1");;
        esac
        shift
    done

    [[ ! "${file_list[*]}" ]] && {
        err "No files"
        exit 2
    }
}

main()
{
    check_args "$@"
    validate_files && draw
}

main "$@"
