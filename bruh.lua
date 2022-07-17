
local DocView = require "core.docview"


local on_mouse_moved = DocView.on_mouse_moved
local draw_line_text = DocView.draw_line_text
local docview_draw = DocView.draw
local docview_update = DocView.update

function DocView:on_mouse_moved(x, y, ...)
  self.last_moved_time = { x, y, system.get_time() }
  self.watch_hover_value = nil
  self.watch_token = nil
  return on_mouse_moved(self, x, y, ...)
end

function DocView:update()
  docview_update(self)
  if not self.watch_calulating and self:is(DocView) and core.active_view == self and not self.watch_hover_value and debugger.state == "stopped" and self.last_moved_time and 
      system.get_time() - math.max(debugger.last_start_time, self.last_moved_time[3]) > config.plugins.debugger.hover_time_watch then
    local x, y = self.last_moved_time[1], self.last_moved_time[2]
    local line, col = self:resolve_screen_position(x, y)
    local _, s = self.doc.lines[line]:reverse():find(config.plugins.debugger.hover_symbol_pattern_backward, #self.doc.lines[line] - col - 1)
    if s then
      local _, e = self.doc.lines[line]:find(config.plugins.debugger.hover_symbol_pattern_forward, col)
      s, e = #self.doc.lines[line] - s + 1, e or col
      local token = self.doc.lines[line]:sub(s, e)
      if #token > 1 and not self.watch_token then
        debugger.print(token, function(result)
          self.watch_hover_value = result:gsub("\\n", "")
        end)
      end
      self.watch_token = { line, s, e }
    end
  end
end

function DocView:draw_line_text(line, x, y)
  if self.watch_token and self.watch_token[1] == line then
    local x1, y = self:get_line_screen_position(line, self.watch_token[2])
    local x2 = self:get_line_screen_position(line, self.watch_token[3] + 1)
    renderer.draw_rect(x1, y, x2 - x1, self:get_line_height(), style.text)
  end
  draw_line_text(self, line, x, y)
end

function DocView:draw()
  docview_draw(self)
  if self.watch_hover_value then
    local x, y = self.last_moved_time[1], self.last_moved_time[2]
    local w, h = style.font:get_width(self.watch_hover_value) + style.padding.x*2, style.font:get_height() + style.padding.y * 2
    renderer.draw_rect(x, y, w, h, style.dim)
    renderer.draw_text(style.font, self.watch_hover_value, x + style.padding.x, y + style.padding.y, style.text)
  end
end
