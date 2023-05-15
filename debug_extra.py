Import("env")

# --- Add custom macros for the ALL files which name contains "main.cpp"
# important change https://github.com/godotengine/godot/issues/74725
# CPPDEFINES nolonger a list, but deque
def addUnixTimestamp(env, node):
  # get a copy of the CPPDEFINES
  mycppdefines = env["CPPDEFINES"].copy()
  # add the unix time stamp
  mycppdefines.append({"BUILD_TIMESTAMP" : "$UNIX_TIME"})
 
  return env.Object(
      node,
      CPPDEFINES=mycppdefines
  )
env.AddBuildMiddleware(addUnixTimestamp, "*\src\main.cpp")

