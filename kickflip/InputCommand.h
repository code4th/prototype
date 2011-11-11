#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <XInput.h>
#include <tchar.h>

#include "SmartPointer.h"
#include "InputDevice.h"
#include "Embedded.h"
#include "Debug/DebugFont.h"

#include <vector>
#include <map>
#include <iterator>

namespace kickflip
{
	SmartPtr(InputCommand);
	class InputCommand :public ReferenceObject
	{
	public:
		InputCommand(void){}

		virtual ~InputCommand(void){}

	public:
		SmartPtr(Action);
		typedef std::map< unsigned int, Action* > ActionMap;
		class Action : public ReferenceObject
		{
		public:
			// このオブジェクトに来てるってことはすでに起動している
			std::string m_kName;

			unsigned int m_uiMixTime;
			unsigned int m_uiMaxTime;

			ActionMap m_kNextActionMap;
		};
	private:
		ActionMap m_kActionMap;			// 関係Map
		ActionRPtr m_kActionStrage;		// オブジェクト保持
	public:
		static ActionRPtr Make(int iMinTime, unsigned int iKey, int iMaxTime, const char* pName)
		{
			ActionRPtr rpAction = new Action;
			rpAction->m_kName = pName;
			rpAction->m_uiMixTime = iMinTime;
			rpAction->m_uiMaxTime = iMaxTime;
			return rpAction;
		}
		bool Regist(ActionRPtr )
		{
		}
		void Analyze(InputDevice::GamePad::InputStateLog kInputStateLog)
		{
			std::copy(kInputStateLog.begin(), kInputStateLog.end(), std::back_inserter(m_kInputStateLog));

			ActionMap& kActionMap = m_kActionMap;
			for(InputDevice::GamePad::InputStateLog::iterator ite=kInputStateLog.begin(); kInputStateLog.end()!=ite;ite++)
			{
				ActionMap::iterator iteFind = kActionMap.find(ite->on);
			}
		}

	public:
		InputDevice::GamePad::InputStateLog m_kInputStateLog;

	};
#if 0
	// コマンド作成
	ActionRPtr kPS = InputCommand::Make(1,GamePad::A,10,"小パンチ");
	ActionRPtr kPL = InputCommand::Make(11,GamePad::A,-1,"大パンチ");
	ActionRPtr kKS = InputCommand::Make(1,GamePad::B,10,"小キック");
	ActionRPtr kKL = InputCommand::Make(11,GamePad::B,-1,"大キック");
	ActionRPtr kNS = InputCommand::Make(1,GamePad::NONE,10,"小ニュートラル");
	ActionRPtr kNL = InputCommand::Make(11,GamePad::NONE,30,"大ニュートラル");

	// PK
	ActionRPtr kPK = InputCommand::Make(kPS+kNS+kKL,"PK");

	// PPK
	State kPPK = InputCommand::Make(kPS+kNS+kPS+kNS+kKL,"PPK");
	State kPPK = InputCommand::Make(kPS+kNS+kPK,"PPK");

	InputCommand::Regist();

// 格ゲーコマンド入力判定
// Fighting game command detection
// http://geoquake.jp/blog/2009/05/20/fightinggame/
//
// Stick        = arrow keys or numpad
// Punch button = Space, Ctrl, Z
// Kick button  = Shift, N, M, X
//
package {
    import flash.display.*;
    import flash.events.*;
    import flash.geom.*;
    import flash.text.*;
    
    [SWF(width = "465", height = "465", frameRate = "50")]
    public class Main extends Sprite {
        private const SCREEN_W:int = 465;
        private const SCREEN_H:int = 465;
        
        private var input:FightingGameInput = new FightingGameInput(stage);
        
        private var technique:TextField = new TextField();
        private var techniqueTicks:int = 0;
        
        private var controlPanel:ControlPanel;
        
        public function Main() {
            // インストラクションを描画
            
            // 0-7    = 矢印(右から時計回り)
            // P    = 「P」
            // K    = 「K」
            // +    = 「+」
            // T    = 「溜め」
            // R    = 「連打」
            // H    = 「離す」
            // N    = 「ニュートラル」
            print("210+P");
            print("021+P");
            print("234+K");
            print("44");
            gotoRight();
            print("4T0+P");
            print("PKT H");
            print("PR");
            
            addChild(new Bitmap(instruction));
            
            controlPanel = new ControlPanel();
            controlPanel.x = SCREEN_W / 2;
            controlPanel.y = 400;
            addChild(controlPanel);
            
            technique.defaultTextFormat = new TextFormat("_ゴシック", 50, 0x00000000, true);
            technique.autoSize = TextFieldAutoSize.CENTER;
            technique.selectable = false;
            technique.x = 30;
            technique.y = 210;
            technique.width = SCREEN_W - 30 * 2;
            addChild(technique);
            
            addEventListener(Event.ENTER_FRAME, enterFrameHandler);
        }
        
        private function enterFrameHandler(event:Event):void {
            input.update();
            
            controlPanel.setState(input.getStick(), 
                input.isDown(FightingGameInput.BUTTON_K), 
                input.isDown(FightingGameInput.BUTTON_P));
            
            function showTechnique(name:String):void {
                technique.text = name;
                techniqueTicks = 40;
            }
        
            // 必殺技チェック！
            // checkCommand(判定フレーム数, コマンドスクリプト)
            if (input.checkCommand(20, "2 1 0P")) {    // 0-7(右から時計回り)でレバーの状態。0P＝右にレバーが入った状態でパンチボタンが押し下げられた
                showTechnique("著作権ッ！");
                input.clearBuffer();
            } else
            if (input.checkCommand(20, "0 2 1P")) {
                showTechnique("肖像権ッ！");
                input.clearBuffer();
            } else
            if (input.checkCommand(20, "2 3 4K")) {
                showTechnique("則巻千兵衛脚ッ！"); // なんかもうほんとごめんなさい
                input.clearBuffer();
            } else
            if (input.checkCommand(10, "N 4 N 4")) { // N＝ニュートラル
                showTechnique("バックステップ");
                input.clearBuffer();
            } else
            if (input.checkCommand(25, "4x20 0P")) { // 4x20＝レバー左が20フレーム
                showTechnique("溜めコマンド");
                input.clearBuffer();
            } else
            if (input.checkCommand(25, "PdKdx20 PuKu")) { // Pd＝パンチボタンが下がっている状態、Pu＝同じく上がっている状態
                showTechnique("溜めコマンド２");
                input.clearBuffer();
            } else
            if (input.checkCommand(30, "P P P P P")) {
                showTechnique("打つべし！打つべし！");
                //input.clearBuffer();
            } else 
            if (input.checkCommand(255, "N 6 N 6 N 2 N 2 N 4 N 0 N 4 N 0 N K P")) {
                showTechnique("コナミコマンド");
                input.clearBuffer();
            } else 
            if (input.isPressed(FightingGameInput.BUTTON_P)) {
                showTechnique("パンチ");
            } else 
            if (input.isPressed(FightingGameInput.BUTTON_K)) {
                showTechnique("キック");
            }
            
            if (techniqueTicks > 0) {
                techniqueTicks--;
                technique.visible = true;
            } else {
                technique.visible = false;
            }
        }
        
        // インストラクションの描画
        private var instruction:BitmapData = new BitmapData(SCREEN_W, SCREEN_H, false, 0xFF9020);
        private var line:int = 33;
        private var instX:int = 33;
        
        private function print(s:String):void {
            var x:int = instX;
            
            var arrow:Shape = new Shape();
            var g:Graphics = arrow.graphics;
            g.clear();
            g.beginFill(0x000000);
            g.drawPath(Vector.<int>([1, 2, 2, 2, 2, 2, 2, 2]), 
                Vector.<Number>([
                    -15, -5,  0, -5,  0, -15,  15, 0,  0, 15,  0, 5,  -15, 5,  -15, -5
                ])
            );
            g.endFill();
            
            var matrix:Matrix = new Matrix();
            
            var textField:TextField = new TextField();
            textField.defaultTextFormat = new TextFormat("_sans", 34, 0x00000000, true);
            textField.autoSize = TextFieldAutoSize.LEFT;
            // ボーダーを表示するとレイアウトが確認しやすい。
            //textField.border = true;
            
            for (var i:int = 0; i < s.length; i++) {
                switch (s.charAt(i)) {
                case "P":
                case "K":
                case "+":
                case " ":
                    textField.text = s.charAt(i);
                    break;
                    
                case "T":
                    textField.text = "溜め";
                    break;
                    
                case "R":
                    textField.text = "連打";
                    break;
                    
                case "H":
                    textField.text = "離す";
                    break;
                    
                case "N":
                    textField.text = "ニュートラル";
                    break;
                    
                default:
                    // 矢印
                    matrix.identity();
                    matrix.rotate(Number(s.charAt(i)) * Math.PI / 4);
                    matrix.translate(x + 15, line + 15);
                    instruction.draw(arrow, matrix);
                    
                    x += 35;
                    continue;
                }
                matrix.identity();
                matrix.translate(x, line - 6);
                instruction.draw(textField, matrix);
                x += textField.width;
            }
            line += 40;
        }
        
        private function gotoRight():void {
            instX = SCREEN_W / 2;
            line = 33;
        }
    }
}

import flash.display.*;
import flash.events.*;
import flash.geom.*;
import flash.ui.*;

class ControlPanel extends Sprite {
    private var stickBall:Shape = new Shape();
    private var stickPole:Shape = new Shape();
    private var buttons:Vector.<Shape> = new Vector.<Shape>();
    
    private const STICK_X:int = -70;
    private const STICK_Y:int = -80;
    private const BUTTON_Y:Array = [ -43, -53 ];
    
    function ControlPanel() {
        var g:Graphics = graphics;
        g.beginFill(0x1E2C2F);
        g.drawRect(-200, -85, 400, 90);
        g.endFill();
        
        g.beginFill(0x000000);
        g.drawEllipse(-90, -60, 40, 30);
        g.endFill();
        
        g.beginFill(0x01792B);
        g.drawEllipse(0, -55, 40, 30);
        g.drawEllipse(45, -65, 40, 30);
        g.endFill();
        
        g = stickBall.graphics;
        var matrix:Matrix = new Matrix();
        matrix.createGradientBox(40, 40, 0, -23, -25);
        g.beginGradientFill(GradientType.RADIAL, [0xFFFFFF, 0x00BD51, 0x01692B], null, [10, 30, 230], matrix);
        g.drawCircle(0, 0, 20);
        g.endFill();
        
        for (var i:int = 0; i < 2; i++) {
            var button:Shape = new Shape();
            g = button.graphics;
            matrix.createGradientBox(34, 24, 0, -17, -13);
            g.beginGradientFill(GradientType.RADIAL, [0x01A03C, 0x01A032, 0x20B040], null, [0, 245, 250], matrix);
            g.drawEllipse( -17, -12, 34, 24);
            g.endFill();
            g.beginFill(0xFFFFFF);
            g.endFill();
            
            buttons.push(button);
        }
        
        addChild(stickPole);
        stickBall.x = STICK_X;
        stickBall.y = STICK_Y;
        addChild(stickBall);
        
        buttons[0].x = 20;
        buttons[0].y = BUTTON_Y[0];
        addChild(buttons[0]);
        
        buttons[1].x = 65;
        buttons[1].y = BUTTON_Y[1];
        addChild(buttons[1]);
    }
    
    public function setState(stick:int, button0:Boolean, button1:Boolean):void {
        var sx:Number = STICK_X;
        var sy:Number = STICK_Y;
        
        if (stick != GameInput.STICK_NEUTRAL) {
            var angle:Number = stick * Math.PI / 4;
            sx = STICK_X + Math.cos(angle) * 10;
            sy = STICK_Y + Math.sin(angle) * 10 + 1;
        }
        stickBall.x = sx;
        stickBall.y = sy;
        
        var g:Graphics = stickPole.graphics;
        g.clear();
        g.lineStyle(8, 0x556666);
        g.moveTo(-70, -45);
        g.lineTo(sx, sy);
        
        buttons[0].y = button0 ? BUTTON_Y[0] + 3 : BUTTON_Y[0];
        buttons[1].y = button1 ? BUTTON_Y[1] + 3 : BUTTON_Y[1];
    }
}

// ゲーム用入力クラス（つくりかけ）
//
// キーボードの状態を毎フレームのupdate()で仮想ボタンの状態に整理し、
// 仮想ボタンの上下左右をgetStick()でスティック方向に整理という流れです。
// スティック方向は0＝右から時計回りで、8(STICK_NEUTRAL)でニュートラルになっています。
// これはベクトルに変換する際便利なためです（テンキー配列も直感的なので迷う……）。
//
// isDown()、isPressed()、isReleased()で、ボタンの押下状態、押した瞬間、離した瞬間を検出します。
// getButtonTicks()でボタンが押されているフレーム数を取得します。
//

// 仮想ボタン
class Button {
    public static const UP:int = 0;
    public static const DOWN:int = 1;
    public static const LEFT:int = 2;
    public static const RIGHT:int = 3;
    public static const A:int = 4;
    public static const B:int = 5;
    public static const MAX:int = 6;
}

class GameInput {
    public static const STICK_NEUTRAL:int = 8;
    
    private var stage:Stage;
    
    private const KEY_CODE_MAX:int = 256;
    private var keyState:Vector.<Boolean> = new Vector.<Boolean>(KEY_CODE_MAX, true);
    
    private var buttonState:Vector.<Boolean> = new Vector.<Boolean>(Button.MAX, true);
    private var buttonTicks:Vector.<int> = new Vector.<int>(Button.MAX, true);
    
    function GameInput(stage:Stage)  {
        this.stage = stage;
        
        clearKeyState();
        for (var i:int = 0; i < Button.MAX; i++) {
            buttonState[i] = false;
            buttonTicks[i] = 0;
        }
        
        stage.addEventListener(KeyboardEvent.KEY_DOWN, keyDownHandler);
        stage.addEventListener(KeyboardEvent.KEY_UP, keyUpHandler);
        stage.addEventListener(Event.DEACTIVATE, deactivateHandler);
    }
    
    private function clearKeyState():void {
        for (var i:int = 0; i < KEY_CODE_MAX; i++) {
            keyState[i] = false;
        }
    }
    
    private function keyDownHandler(event:KeyboardEvent):void {
        keyState[event.keyCode] = true;
    }
    private function keyUpHandler(event:KeyboardEvent):void {
        keyState[event.keyCode] = false;
    }
    private function deactivateHandler(event:Event):void {
        clearKeyState();
    }
    
    private const TO_STICK:Array = [ -1, 3, 2, 1, 4, STICK_NEUTRAL, 0, 5, 6, 7 ];
    
    public function getStick():int {
        // 一旦テンキー配列にする。
        var d:int = 5;
        if (isDown(Button.UP)) {
            d += 3;
        } else if (isDown(Button.DOWN)) {
            d -= 3;
        }
        if (isDown(Button.RIGHT)) {
            d += 1;
        } else if (isDown(Button.LEFT)) {
            d -= 1;
        }
        // テンキー配列を時計回り配列に変換
        return TO_STICK[d];
    }
    
    public function getButtonTicks(button:int):int {
        return buttonTicks[button];
    }
    
    public function isDown(button:int):Boolean {
        return (getButtonTicks(button) > 0);
    }
    public function isPressed(button:int):Boolean {
        return (getButtonTicks(button) == 1);
    }
    public function isReleased(button:int):Boolean {
        return (getButtonTicks(button) < 0);
    }
    
    public function update():void {
        buttonState[Button.UP] = keyState[Keyboard.UP] || keyState[Keyboard.NUMPAD_8];
        buttonState[Button.DOWN] = keyState[Keyboard.DOWN] || keyState[Keyboard.NUMPAD_2];
        buttonState[Button.LEFT] = keyState[Keyboard.LEFT] || keyState[Keyboard.NUMPAD_4];
        buttonState[Button.RIGHT] = keyState[Keyboard.RIGHT] || keyState[Keyboard.NUMPAD_6];
        
        buttonState[Button.A] = keyState[Keyboard.SPACE] || keyState[Keyboard.CONTROL] || keyState[90];
        buttonState[Button.B] = keyState[Keyboard.SHIFT] || keyState[78] || keyState[77] || keyState[88];
        
        for (var i:int = 0; i < Button.MAX; i++) {
            if (buttonState[i]) {
                buttonTicks[i]++;
            } else {
                if (buttonTicks[i] > 0) {
                    buttonTicks[i] = -buttonTicks[i];
                } else if (buttonTicks[i] < 0) {
                    buttonTicks[i] = 0;
                }
            }
        }
    }
}

// GameInputクラスに格ゲーコマンド判定を付加
class FightingGameInput extends GameInput {
    // ボタン別名
    public static const BUTTON_P:int = Button.A;
    public static const BUTTON_K:int = Button.B;
    
    // 入力状態を溜め込むリングバッファ
    private const BUFFER_SIZE:int = 256; // 2の乗数
    private var buffer:Vector.<uint> = new Vector.<uint>(BUFFER_SIZE, true);
    private var cursor:int = 0;
    
    // bufferの要素の構造
    // |76543210|76543210|76543210|76543210|
    // |        |        |KKKKPPPP|<stick >|
    //                       /   |
    //            +--------+-------+--+----+
    //          |released|pressed|up|down|
    //            +--------+-------+--+----+
    
    function FightingGameInput(stage:Stage) {
        super(stage);
        clearBuffer();
    }
    
    public function clearBuffer():void {
        for (var i:int = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = STICK_NEUTRAL;
        }
    }
    
    public override function update():void {
        super.update();
        
        buffer[cursor] = getStick()
            | (int(isDown(BUTTON_P))     <<  8)
            | (int(!isDown(BUTTON_P))    <<  9)
            | (int(isPressed(BUTTON_P))  << 10)
            | (int(isReleased(BUTTON_P)) << 11)
            | (int(isDown(BUTTON_K))     << 12)
            | (int(!isDown(BUTTON_K))    << 13)
            | (int(isPressed(BUTTON_K))  << 14)
            | (int(isReleased(BUTTON_K)) << 15);
        if (++cursor >= BUFFER_SIZE) {
            cursor = 0;
        }
    }
    
    public function checkCommand(ticks:int, command:String):Boolean {
        if (ticks >= BUFFER_SIZE) {
            throw Error("too long ticks");
        }
        
        //    メモ
        //  p                cursor
        //  v                v
        //    88833333333882288
        
        var p:int = cursor - ticks;
        
        function search(state:uint, mask:uint, length:int):Boolean {
            var matchLength:int = 0;
            
            for (; (p & (BUFFER_SIZE - 1)) != cursor; p++) {
                if ((buffer[p & (BUFFER_SIZE - 1)] & mask) == state) {
                    matchLength++;
                    if (matchLength >= length) {
                        p++;
                        return true;
                    }
                } else {
                    matchLength = 0;
                }
            }
            // バッファ末端までマッチしなかった。
            return false;
        }
        
        var rex:RegExp = /([0-8NPKudpr]+)(x\d+)?/g;
        for (var result:Object = rex.exec(command); result; result = rex.exec(command)) {
            var length:int = result[2] ? result[2].substring(1) : 1;
            
            var state:uint = 0x0000;
            var mask:uint = 0x0000;
            
            var r:RegExp = /([0-8N]|[PK])([udpr])?/g;
            for (var button:Object = r.exec(result[1]); button; button = r.exec(result[1])) {
                var shift:int = 2;
                switch (button[2]) {
                case "d": // down
                    shift = 0;
                    break;
                case "u": // up
                    shift = 1;
                    break;
                case "p": // pressed
                    shift = 2;
                    break;
                case "r": // released
                    shift = 3;
                    break;
                }
                
                if (button[1] == "N") {
                    button[1] = "8";
                }
                switch (button[1]) {
                case "P":
                    shift += 8;
                    mask |= 1 << shift;
                    state |= 1 << shift;
                    break;
                case "K":
                    shift += 12;
                    mask |= 1 << shift;
                    state |= 1 << shift;
                    break;
                default:
                    mask |= 0xFF;
                    state |= button[1];
                    break;
                }
            }
            if (!search(state, mask, length)) {
                return false;
            }
        }
        return true;
    }
}




#endif
}

