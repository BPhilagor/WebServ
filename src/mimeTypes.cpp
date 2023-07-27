
#include <map>
#include "mimeTypes.hpp"

static std::map<std::string, std::string> init_mimetypes();

const std::string &getMimeFromExtension(std::string& ext)
{
	static std::map<std::string, std::string> mime = init_mimetypes();

	return mime[ext];
}

static std::map<std::string, std::string> init_mimetypes()
{
	std::map<std::string, std::string> mime;

	mime["html"] = "text/html";
	mime["htm"] = "text/html";
	mime["shtml"] = "text/html";
	mime["css"] = "text/css";
	mime["xml"] = "text/xml";
	mime["gif"] = "image/gif";
	mime["jpeg"] = "image/jpeg";
	mime["jpeg"] = "image/jpeg";
	mime["js"] = "application/javascript";
	mime["atom"] = "application/atom+xml";
	mime["rss"] = "application/rss+xml";
	mime["mml"] = "text/mathml";
	mime["txt"] = "text/plain";
	mime["jad"] = "text/vnd.sun.j2me.app-descriptor";
	mime["wml"] = "text/vnd.wap.wml";
	mime["htc"] = "text/x-component";
	mime["avif"] = "image/avif";
	mime["png"] = "image/png";
	mime["svgz"] = "image/svg+xml";
	mime["svg"] = "image/svg+xml";
	mime["tif"] = "image/tiff";
	mime["tiff"] = "image/tiff";
	mime["wbmp"] = "image/vnd.wap.wbmp";
	mime["webp"] = "image/webp";
	mime["ico"] = "image/x-icon";
	mime["jng"] = "image/x-jng";
	mime["bmp"] = "image/x-ms-bmp";
	mime["woff"] = "font/woff";
	mime["woff2"] = "font/woff2";
	mime["jar"] = "application/java-archive";
	mime["war"] = "application/java-archive";
	mime["ear"] = "application/java-archive";
	mime["json"] = "application/json";
	mime["hqx"] = "application/mac-binhex40";
	mime["doc"] = "application/msword";
	mime["pdf"] = "application/pdf";
	mime["eps"] = "application/postscript";
	mime["ps"] = "application/postscript";
	mime["ai"] = "application/postscript";
	mime["rtf"] = "application/rtf";
	mime["m3u8"] = "application/vnd.apple.mpegurl";
	mime["kml"] = "application/vnd.google-earth.kml+xml";
	mime["kmz"] = "application/vnd.google-earth.kmz";
	mime["xls"] = "application/vnd.ms-excel";
	mime["eot"] = "application/vnd.ms-fontobject";
	mime["ppt"] = "application/vnd.ms-powerpoint";
	mime["odg"] = "application/vnd.oasis.opendocument.graphics";
	mime["odp"] = "application/vnd.oasis.opendocument.presentation";
	mime["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mime["odt"] = "application/vnd.oasis.opendocument.text";
	mime["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mime["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mime["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mime["wmlc"] = "application/vnd.wap.wmlc";
	mime["wasm"] = "application/wasm";
	mime["7z"] = "application/x-7z-compressed";
	mime["cco"] = "application/x-cocoa";
	mime["jardiff"] = "application/x-java-archive-diff";
	mime["jnlp"] = "application/x-java-jnlp-file";
	mime["run"] = "application/x-makeself";
	mime["pm"] = "application/x-perl";
	mime["pl"] = "application/x-perl";
	mime["prc"] = "application/x-pilot";
	mime["pdb"] = "application/x-pilot";
	mime["rar"] = "application/x-rar-compressed";
	mime["rpm"] = "application/x-redhat-package-manager";
	mime["sea"] = "application/x-sea";
	mime["swf"] = "application/x-shockwave-flash";
	mime["sit"] = "application/x-stuffit";
	mime["tcl"] = "application/x-tcl";
	mime["tk"] = "application/x-tcl";
	mime["der"] = "application/x-x509-ca-cert";
	mime["pem"] = "application/x-x509-ca-cert";
	mime["crt"] = "application/x-x509-ca-cert";
	mime["xpi"] = "application/x-xpinstall";
	mime["xhtml"] = "application/xhtml+xml";
	mime["xspf"] = "application/xspf+xml";
	mime["zip"] = "application/zip";
	mime["bin"] = "application/octet-stream";
	mime["exe"] = "application/octet-stream";
	mime["dll"] = "application/octet-stream";
	mime["deb"] = "application/octet-stream";
	mime["dmg"] = "application/octet-stream";
	mime["iso"] = "application/octet-stream";
	mime["img"] = "application/octet-stream";
	mime["msi"] = "application/octet-stream";
	mime["msp"] = "application/octet-stream";
	mime["msm"] = "application/octet-stream";
	mime["mid"] = "audio/midi";
	mime["midi"] = "audio/midi";
	mime["kar"] = "audio/midi";
	mime["mp3"] = "audio/mpeg";
	mime["ogg"] = "audio/ogg";
	mime["m4a"] = "audio/x-m4a";
	mime["ra"] = "audio/x-realaudio";
	mime["3gp"] = "video/3gpp";
	mime["3gpp"] = "video/3gpp";
	mime["ts"] = "video/mp2t";
	mime["mp4"] = "video/mp4";
	mime["mpg"] = "video/mpeg";
	mime["mpeg"] = "video/mpeg";
	mime["mov"] = "video/quicktime";
	mime["webm"] = "video/webm";
	mime["flv"] = "video/x-flv";
	mime["m4v"] = "video/x-m4v";
	mime["mng"] = "video/x-mng";
	mime["asx"] = "video/x-ms-asf";
	mime["asf"] = "video/x-ms-asf";
	mime["wmv"] = "video/x-ms-wmv";
	mime["avi"] = "video/x-msvideo";
	return mime;
}

/*
types {
    text/html                                        html htm shtml;
    text/css                                         css;
    text/xml                                         xml;
    image/gif                                        gif;
    image/jpeg                                       jpeg jpg;
    application/javascript                           js;
    application/atom+xml                             atom;
    application/rss+xml                              rss;

    text/mathml                                      mml;
    text/plain                                       txt;
    text/vnd.sun.j2me.app-descriptor                 jad;
    text/vnd.wap.wml                                 wml;
    text/x-component                                 htc;

    image/avif                                       avif;
    image/png                                        png;
    image/svg+xml                                    svg svgz;
    image/tiff                                       tif tiff;
    image/vnd.wap.wbmp                               wbmp;
    image/webp                                       webp;
    image/x-icon                                     ico;
    image/x-jng                                      jng;
    image/x-ms-bmp                                   bmp;

    font/woff                                        woff;
    font/woff2                                       woff2;

    application/java-archive                         jar war ear;
    application/json                                 json;
    application/mac-binhex40                         hqx;
    application/msword                               doc;
    application/pdf                                  pdf;
    application/postscript                           ps eps ai;
    application/rtf                                  rtf;
    application/vnd.apple.mpegurl                    m3u8;
    application/vnd.google-earth.kml+xml             kml;
    application/vnd.google-earth.kmz                 kmz;
    application/vnd.ms-excel                         xls;
    application/vnd.ms-fontobject                    eot;
    application/vnd.ms-powerpoint                    ppt;
    application/vnd.oasis.opendocument.graphics      odg;
    application/vnd.oasis.opendocument.presentation  odp;
    application/vnd.oasis.opendocument.spreadsheet   ods;
    application/vnd.oasis.opendocument.text          odt;
    application/vnd.openxmlformats-officedocument.presentationml.presentation                                                     pptx;
    application/vnd.openxmlformats-officedocument.spreadsheetml.sheet                                                     xlsx;
    application/vnd.openxmlformats-officedocument.wordprocessingml.document
                                                     docx;
    application/vnd.wap.wmlc                         wmlc;
    application/wasm                                 wasm;
    application/x-7z-compressed                      7z;
    application/x-cocoa                              cco;
    application/x-java-archive-diff                  jardiff;
    application/x-java-jnlp-file                     jnlp;
    application/x-makeself                           run;
    application/x-perl                               pl pm;
    application/x-pilot                              prc pdb;
    application/x-rar-compressed                     rar;
    application/x-redhat-package-manager             rpm;
    application/x-sea                                sea;
    application/x-shockwave-flash                    swf;
    application/x-stuffit                            sit;
    application/x-tcl                                tcl tk;
    application/x-x509-ca-cert                       der pem crt;
    application/x-xpinstall                          xpi;
    application/xhtml+xml                            xhtml;
    application/xspf+xml                             xspf;
    application/zip                                  zip;

    application/octet-stream                         bin exe dll;
    application/octet-stream                         deb;
    application/octet-stream                         dmg;
    application/octet-stream                         iso img;
    application/octet-stream                         msi msp msm;

    audio/midi                                       mid midi kar;
    audio/mpeg                                       mp3;
    audio/ogg                                        ogg;
    audio/x-m4a                                      m4a;
    audio/x-realaudio                                ra;

    video/3gpp                                       3gpp 3gp;
    video/mp2t                                       ts;
    video/mp4                                        mp4;
    video/mpeg                                       mpeg mpg;
    video/quicktime                                  mov;
    video/webm                                       webm;
    video/x-flv                                      flv;
    video/x-m4v                                      m4v;
    video/x-mng                                      mng;
    video/x-ms-asf                                   asx asf;
    video/x-ms-wmv                                   wmv;
    video/x-msvideo                                  avi;
}
*/
