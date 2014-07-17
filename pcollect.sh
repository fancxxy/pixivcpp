#!/bin/bash

USER_AGENT='Mozilla/5.0 (X11; Linux 86_64; rv:30.0) Gecko/20100101 Firefox/30.0'
ACCOUNT=''
PASSWORD=''
COOKIE='/tmp/pixiv-cookie'

#保存图像基本信息
pixiv_img_msg=

#保存画师基本信息
pixiv_painter_msg=

#登录
get_cookie() {
    curl -s -c $COOKIE -A "$USER_AGENT" -d "mode=login&pixiv_id=$ACCOUNT&pass=$PASSWORD&skip=1" \
        "http://www.pixiv.net/login.php"
}

#输出
info() {
    printf "\e[1;32m%-7s\e[0m: " $1
}

#获取id对应图像所在页面信息
#参数 作品id
get_img_msg() {
    pixiv_img_msg=$(curl -s -b $COOKIE -A "$USER_AGENT" "http://www.pixiv.net/member_illust.php?mode=medium&illust_id=$1")
}

#模式，单张还是漫画还是动态图
get_img_mode() {
    local pixiv_img_mode_regex='^.*<a href="/\{0,1\}member_illust.php?mode=\([^&]\{1,\}\)&amp;illust_id=[[:digit:]]\{1,\}" target="_blank".*$'
    local pixiv_img_mode_find='works_display'
    echo "$pixiv_img_msg" | grep "$pixiv_img_mode_find" | sed "s|$pixiv_img_mode_regex|\1|" 
}

#图片标题
get_img_title() {
    local pixiv_img_title_regex='^.*「\(.*\)」/.*$'
    echo "$pixiv_img_msg" | grep "<title>" | sed "s|$pixiv_img_title_regex|\1|"

}

#画师名字
get_img_painter() {
    local pixiv_img_painter_regex='^.*/「\(.*\)」.*$'
    echo "$pixiv_img_msg" | grep "<title>" | sed "s|$pixiv_img_painter_regex|\1|"
}

#图像所有标签
get_img_tag() {
    local pixiv_img_tag_regex='class=\"text\">\([^<]\{1,\}\)</a>'
    echo "$pixiv_img_msg" | grep -o "$pixiv_img_tag_regex" | sed "s|$pixiv_img_tag_regex|\1|" | xargs
}

#输出图像下载地址
#参数 mode, 作品id
get_img_url() {
    local pixiv_img_big_url_regex='http://i[12]\.pixiv\.net/\([^/]\{1,\}/\)\{3\}[[:digit:]]\{1,\}\.[[:alpha:]]\{1,\}\(?[[:digit:]]\{1,\}\)\{0,1\}'
    local pixiv_img_manga_url_regex='data-src="http://i[12]\.pixiv\.net/\([^/]\{1,\}/\)\{3\}[[:digit:]]\{1,\}_p[[:digit:]]\{1,\}\.[[:alpha:]]\{1,\}'
    local pixiv_img_ugoira_view_url_regex='http:\\/\\/i[12]\.pixiv\.net\\/\([^\/]\{1,\}\\/\)\{8\}[[:digit:]]\{1,\}_ugoira1920x1080\.zip'

    if [[ $1 = "big" ]];
    then
        curl -s -b $COOKIE -A "$USER_AGENT" -e "http://www.pixiv.net/member_illust.php?mode=medium&illust_id=$2" \
            "http://www.pixiv.net/member_illust.php?mode=big&illust_id=$2" | \
            grep -o "$pixiv_img_big_url_regex"
    elif [[ $1 = "manga" ]];
    then
        curl -s -b $COOKIE -A "$USER_AGENT" -e "http://www.pixiv.net/member_illust.php?mode=medium&illust_id=$2" \
            "http://www.pixiv.net/member_illust.php?mode=manga&illust_id=$2" | \
            grep -o "$pixiv_img_manga_url_regex" | \
            sed 's/data-src="//g'
    elif [[ $1 = "ugoira_view" ]];
    then
        curl -s -b $COOKIE -A "$USER_AGENT" -e "http://www.pixiv.net/member_illust.php?id=$2" \
            "http://www.pixiv.net/member_illust.php?mode=medium&illust_id=$2" | \
            grep -o "$pixiv_img_ugoira_view_url_regex" | \
            sed 's/\\//g'
    else
        echo nothing
    fi
}

#处理动态gif
#参数 压缩包名
process_ugoira_view() {
    pixiv_img_ugoira_view_name=$(echo $1 | sed "s/_ugoira1920x1080.zip//")
    unzip -q $1 -d /tmp/$1
    convert -delay 0 -loop 0 /tmp/$1/* ${pixiv_img_ugoira_view_name}.gif
    # convert -delay <帧数>x<每秒帧数> -loop 循环次数 inputdir output.gif
    rm -rf /tmp/$1 $1
}


#输出图像信息
info_img_msg() {
    echo "--------------------------------------"
    info "painter"
    get_img_painter
    info "title"
    get_img_title
    info "tag"
    get_img_tag
    echo "--------------------------------------"
}

#下载单个图片
#参数 id
download_single_img() {
    local pixiv_img_mode=$(get_img_mode)
    local pixiv_img_url=$(get_img_url $pixiv_img_mode $1)
    for url in $pixiv_img_url;
    do
        pixiv_img_name=$(curl -O -w %{filename_effective} -b "$COOKIE" -A "$USER_AGENT" \
            -e "http://www.pixiv.net/member_illust.php?mode=$pixiv_img_mode&illust_id=$1" "$url")
    done

    if [[ $pixiv_img_mode = "ugoira_view" ]];
    then
        process_ugoira_view $pixiv_img_name
    fi
}

#获取画师主页信息
#参数 画师id
get_painter_msg() {
    pixiv_painter_msg=$(curl -s -b $COOKIE -A "$USER_AGENT" "http://www.pixiv.net/member.php?id=$1")
}

#画师名字
get_painter_name() {
    local pixiv_painter_name_regex='^.*「\(.*\)」.*$'
    echo "$pixiv_painter_msg" | grep "<title>" | sed "s|$pixiv_painter_name_regex|\1|"
}

#画师作品数
get_painter_count() {
    local pixiv_painter_count_regex='id=[[:digit:]]\{1,\}\">[^(]\{1,\}(\([[:digit:]]\{1,\}\))</a>'
    echo "$pixiv_painter_msg" | grep -o -m 1 "$pixiv_painter_count_regex" | sed "s|$pixiv_painter_count_regex|\1|"
}

#作品页数
get_painter_page() {
    pixiv_painter_count=$(get_painter_count)
    expr \( $pixiv_painter_count + 20 \) / 20
}

#获取画师某一页作品所有id列表
#参数 画师id 某页数 
get_painter_list() {
    local pixiv_painter_list_regex="illust_id=\([[:digit:]]\{1,\}\)"
    curl -s -b $COOKIE -A "$USER_AGENT" -e "http://www.pixiv.net" \
        "http://www.pixiv.net/member_illust.php?id=$1&p=$2" | \
        grep -o "$pixiv_painter_list_regex" | \
        sed "s|$pixiv_painter_list_regex|\1|"
}

#下载画师某一页的所有作品
#参数 画师id 页数
download_painter_list() {
    local pixiv_painter_list=
    pixiv_painter_list=$(get_painter_list $1 $2)
    for id in $pixiv_painter_list;
    do
        get_img_msg $id
        download_single_img $id
    done
}

#下载(m, n)页的作品
#参数 画师id, 页数(m, n)
download_painter_page() {
    for((page=$2; page<=$3; page++)) {
        echo "page $page"
        download_painter_list $1 $page
    }
}

#输出画师信息
info_painter_msg() {
    echo "--------------------------------------"
    info "painter"
    get_painter_name
    info "works"
    get_painter_count
    info "page"
    get_painter_page
    echo "--------------------------------------"
}

main() {
    get_cookie
    get_img_msg $1
    info_img_msg
    download_single_img $1

    #if [[ $1 = "-p" ]];
    #then
    #    get_img_msg $2
    #    info_img_msg
    #    download_single_img $2
    #elif [[ $1 = "-m" ]]; 
    #then
    #   get_painter_msg $2
    #   info_painter_msg
    #   download_painter_page $2 $3 $4
    #fi
}

main $@
