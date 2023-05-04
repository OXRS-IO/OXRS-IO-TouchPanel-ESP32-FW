Import("env")

# --- Add custom macros for the ALL files which name contains "main.cpp"
def addUnixTimestamp(env, node):

  return env.Object(
      node,
      CPPDEFINES=env["CPPDEFINES"]
      + [("BUILD_TIMESTAMP", "$UNIX_TIME")]
  )

env.AddBuildMiddleware(addUnixTimestamp, "*\src\main.cpp")

