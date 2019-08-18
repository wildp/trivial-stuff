# Copyright 2018 Peter Wild
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#Lightcycles
#By Peter Wild
#Improved and fixed version

### Game settings     ###
player_count  = 4       # Normally 2 but could be 4. 3 is unfair, and 1 is snake without food.
style_name    = "dark"  # Can be either "dark" or "lght".
destroy_trail = False   # Can be either True or False.
scale         = 8       # Default = 8    NOTE: scale * grid should be smaller than the monitor resolution.
grid          = 100     # Default = 100  NOTE: Ideally an even number.
midway        = grid/2  # Default = 50   NOTE: Should be the midpoint of grid.
tick_speed    = 30      # Default = 30   NOTE: Larger grids should have higher tick_speed.
### Game settings END ###

import pygame


pygame.init()

style         = ([0,1] if (style_name == "dark") else ([1,0] if (style_name == "lght") else [0,0]))
destroy_trail = (False if (destroy_trail == False) else (False if (player_count <= 2) else True))
midway        = int(midway)
game_time     = pygame.time.Clock()
window        = pygame.display.set_mode((scale*(grid+1),scale*(grid+1)))

pygame.display.set_caption("Lightcycles")


class C:  # for constants
    start     = [[     0, grid - midway, "r"],
                 [  grid, midway, "l"],
                 [midway,      0, "d"],
                 [grid - midway,   grid, "u"]]

    colours   = [pygame.Color(  0,  0,  0),
                 pygame.Color(255,255,255)]

    players   = [["Blue"  , pygame.Color(  0, 191, 255)],
                 ["Orange", pygame.Color(255, 127,   0)],
                 ["Green" , pygame.Color(  0, 191,   0)],
                 ["Purple", pygame.Color(191,   0, 255)]]

    controls  = [[100 , 97, 119, 115], #WASD        - Blue
                 [275, 276, 273, 274], #ARROW KEYS  - Orange
                 [262, 260, 264, 261], #NUMPAD 8456 - Green
                 [107, 104, 117, 106]] #UHJK        - Purple



class Player:
    def __init__(self, player_no, startx, starty, startdir):
        self.player_no = player_no
        self.location  =  [startx,starty]
        self.trail     = [[startx,starty]]
        self.direction = startdir
        self.alive     = True
        return
    
    def move(self, cd):
        if self.alive == True:
            cd = cd[self.player_no]
            
            if cd == "r" and not self.direction == "l":
                self.direction = cd
            if cd == "l" and not self.direction == "r":
                self.direction = cd
            if cd == "u" and not self.direction == "d":
                self.direction = cd
            if cd == "d" and not self.direction == "u":
                self.direction = cd

            if self.direction == "r":
                self.location[0] += 1
            if self.direction == "l":
                self.location[0] -= 1
            if self.direction == "u":
                self.location[1] -= 1
            if self.direction == "d":
                self.location[1] += 1

            self.trail.insert(0, [self.location[0], self.location[1]])
        return

    def collide(self, global_trails): #collison detction help needed
        if self.alive == True:
            for i in range(len(global_trails)):
                for j in (global_trails[i][1:] if (i == self.player_no) else global_trails[i]):
                    if self.location[0] == j[0] and self.location[1] == j[1]:
                        if C.players[self.player_no][0] == C.players[i][0]:
                            print("Player",C.players[self.player_no][0],"has committed suicide.")
                        else:
                            print("Player",C.players[self.player_no][0],"was killed by Player",str(C.players[i][0])+".")
                        self.die(global_trails)
                        return
            
            for k in range(2):
                if self.location[k] > grid or self.location[k] < 0:
                    print("Player",C.players[self.player_no][0],"has died.")
                    self.die(global_trails)
                    return
        elif self.alive == None: self.die(global_trails)
        return

    def die(self, global_trails):
        if self.alive == None:
            self.alive = False
            if destroy_trail == True:
                self.trail.clear()
            else:
                status = 0
                for i in range(len(global_trails)):
                    if i != self.player_no:
                        if self.location[0] == global_trails[i][0][0] and self.location[1] == global_trails[i][0][1]:
                            status = 1
                if status == 0:
                    del self.trail[0]
        else:
            self.alive = None
        return



def determine_win_text(a):
    if a.count(False) == (player_count - 1):
        for i in range(player_count):
            if a[i] == True:
                x =  (str(C.players[i][0]) + " Wins")
                print("Player",C.players[i][0],"has won the game.\n")
    elif a.count(False) == player_count:
        x = "No One Wins"
        print("No one has won the game.")
    else:
        x = None
    return x

def render_text(w_text, alive):
    if alive.count(False) >= (player_count - 1):

        if w_text == None:
            print("\nGAME OVER")
            w_text = determine_win_text(alive)
        
        w_font = pygame.font.Font("freesansbold.ttf", int(10*scale*(grid/100)))
        w_surf = w_font.render(w_text, True, C.colours[style[1]])
        w_rect = w_surf.get_rect()
        w_rect.midtop = (midway * scale, (int(grid/10)+1)* scale)
        window.blit(w_surf, w_rect)
        return w_text

def update(obj):
    trails = []
    alive  = []
    for o in obj:
        trails.append(o.trail)
        alive.append(o.alive)
    return [trails, alive]

def main():

    p        = []
    cd       = []
    updates  = [[], []]
    win_text = None
    close    = False
    print("GAME STARTED\n")

    for i in range(player_count):
        cd.append(C.start[i][2])
        p.append(Player(i, C.start[i][0], C.start[i][1], C.start[i][2]))
    
    while True:
        
        for event in pygame.event.get():
            if event.type == 12:
                close = True
            
            elif event.type == 2:
                if event.key == 27:
                    close = True
                for i in range(player_count):
                    if event.key == C.controls[i][0]:
                        cd[i] = "r"
                    if event.key == C.controls[i][1]:
                        cd[i] = "l"
                    if event.key == C.controls[i][2]:
                        cd[i] = "u"
                    if event.key == C.controls[i][3]:
                        cd[i] = "d"

        for players in p:
            players.move(cd)
        
        updates = update(p)

        for players in p:
            players.collide(updates[0])
        
        updates = update(p)

        #DRAWING SECTION
        window.fill(C.colours[style[0]])
        for i in range(len(updates[0])):
            for j in updates[0][i]:
                pygame.draw.rect(window, C.players[i][1], pygame.Rect(j[0]*scale, j[1]*scale, scale, scale))
            
        win_text = render_text(win_text, updates[1])
        
        pygame.display.flip()
        game_time.tick(tick_speed)

        if close == True:
            if __name__ == "__main__":
                pygame.quit()
                quit()
            return

if __name__ == "__main__":
    main()
