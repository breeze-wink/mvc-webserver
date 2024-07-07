document.getElementById('uploadForm').addEventListener('submit', function (event) {
    event.preventDefault();
    const formData = new FormData(event.target);
    const xhr = new XMLHttpRequest();
    const progressBar = document.getElementById('progressBar');
    const progressBarFill = progressBar.querySelector('div');
    progressBar.style.display = 'block';
    xhr.open('POST', '/file/upload', true);

    xhr.upload.addEventListener('progress', function (e) {
        if (e.lengthComputable) {
            const percentComplete = (e.loaded / e.total) * 100;
            progressBarFill.style.width = percentComplete + '%';
            progressBarFill.textContent = Math.round(percentComplete) + '%';
        }
    });

    xhr.onload = function () {
        if (xhr.status === 200) 
        {
            const response = JSON.parse(xhr.responseText);
            progressBarFill.style.width = '100%';
            progressBarFill.textContent = '100%';
            document.getElementById('uploadResult').innerHTML = `
                <div class="success-message">
                    文件上传成功！<br />
                    文件名：<span class="file-info">${response.filename}</span><br />
                    文件大小：<span class="file-info">${(response.size / 1024).toFixed(2)} KB</span>
                </div>
            `;
        } 
        else {
            document.getElementById('uploadResult').innerHTML = `
                <div class="success-message" style="background-color: #f2dede; color: #a94442; border-color: #ebccd1;">
                    文件上传失败，请重试。
                </div>
            `;
        }
        progressBar.style.display = 'none';
    };

    xhr.send(formData);
});
