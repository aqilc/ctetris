-- Put project's module settings here.
-- This module will be loaded when opening a project, after the user module
-- configuration.
-- It will be automatically reloaded when saved.

local config = require "core.config"

-- you can add some patterns to ignore files within the project
config.ignore_files = {"^%.", "build", "out"}

-- Patterns are normally applied to the file's or directory's name, without
-- its path. See below about how to apply filters on a path.
--
-- Here some examples:
--
-- "^%." match any file of directory whose basename begins with a dot.
--
-- When there is an '/' or a '/$' at the end the pattern it will only match
-- directories. When using such a pattern a final '/' will be added to the name
-- of any directory entry before checking if it matches.
--
-- "^%.git/" matches any directory named ".git" anywhere in the project.
--
-- If a "/" appears anywhere in the pattern except if it appears at the end or
-- is immediately followed by a '$' then the pattern will be applied to the full
-- path of the file or directory. An initial "/" will be prepended to the file's
-- or directory's path to indicate the project's root.
--
-- "^/node_modules/" will match a directory named "node_modules" at the project's root.
-- "^/build.*/" match any top level directory whose name begins with "build"
-- "^/subprojects/.+/" match any directory inside a top-level folder named "subprojects".

-- You may activate some plugins on a pre-project base to override the user's settings.
-- config.plugins.trimwitespace = true
