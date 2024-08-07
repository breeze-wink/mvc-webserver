#include "file.h"
#include "file_upload.h"
#include "system.h"
using namespace breeze::app;

CONTROLLER(File);
ACTION(File, index);
ACTION(File, upload);


void File::index(const Request& req, Response& resp)
{
    resp.render("file/index.html");
}

void File::upload(const Request& req, Response& resp)
{
    FileUpload file = req.file("file");

    auto sys = Singleton<System>::Instance();

    const string path = sys -> get_root_path() + "/upload/" + file.filename();

    file.save(path);

    Json json;
    json["message"] = "文件上传成功";
    json["filename"] = file.filename();
    json["size"] = file.size();

    resp.json(json.str());
}