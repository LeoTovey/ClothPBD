import subprocess
import os

# 运行 webidl_binder.py 命令
subprocess.run(["python", "deps/emscripten/tools/webidl_binder.py", "cloth_simulator.idl", "glue"])

print("Hello World!")

# 创建并进入 build 目录
os.makedirs("build", exist_ok=True)
os.chdir("build")

# 运行 emcmake cmake ..
subprocess.run(["emcmake", "cmake", ".."])

# 运行 cmake --build .
subprocess.run(["cmake", "--build", "."])


# 返回上一级目录
os.chdir("..")


# 读取文件内容
with open('build/PBD.js', 'r') as file:
    file_content = file.read()

# 执行替换操作
new_content = file_content.replace("const { createRequire } = await import",
 "const { createRequire } = await require")

# 将替换后的内容写回文件
with open('build/PBD.js', 'w') as file:
    file.write(new_content)


subprocess.run(["cp", "build/PBD.js", "../src/"])
subprocess.run(["cp", "build/PBD.wasm", "../src/"])
subprocess.run(["npm", "run", "dev"])