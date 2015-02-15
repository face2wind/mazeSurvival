package view
{
	import event.SceneEvent;
	
	import face2wind.customUIComponents.Message;
	import face2wind.event.ParamEvent;
	import face2wind.manager.RenderManager;
	import face2wind.manager.TimerManager;
	import face2wind.manager.item.IRender;
	import face2wind.view.BaseSprite;
	
	import findPath.AStarPathFinder;
	
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import model.SceneManager;
	import model.vo.MonsterVo;
	import model.vo.PlayerVo;
	
	/**
	 * 场景
	 * @author face2wind
	 */
	public class Scene extends BaseSprite implements IRender
	{
		/**
		 * 网格 
		 */		
		private var meshShape:Shape;
		
		private var manager:SceneManager;
		private var pathFinder:AStarPathFinder;
		/**
		 * 地图背景元素层 
		 */		
		private var elementLayer:BaseSprite;
		/**
		 * 路径层 
		 */		
		private var pathLayer:BaseSprite;
		/**
		 * 生物层 
		 */		
		private var liveLayer:BaseSprite;
		
		/**
		 * 临时变量，寻好的待显示路径 
		 */		
		private var _tmpFindPathArr:Array;
		
		private var playerDic:Dictionary;
		private var monsterDic:Dictionary;
		
		public function Scene()
		{
			super();
		}
		
		/**
		 * 此函数是视图的内容初始化函数<br/>对父类的覆盖 
		 */		
		protected override function createChildren():void
		{
			super.createChildren();
			meshShape = new Shape();
			addChild(meshShape);
			elementLayer = new BaseSprite();
			addChild(elementLayer);
			pathLayer = new BaseSprite();
			addChild(pathLayer);
			liveLayer = new BaseSprite();
			addChild(liveLayer);
			
			playerDic = new Dictionary();
			monsterDic = new Dictionary();
			
			manager = SceneManager.getInstance();
			pathFinder = new AStarPathFinder();
		}
		
		/**
		 * 清空地图 
		 */		
		private function clearMaze():void
		{
			meshShape.graphics.clear();
		}
		
		/**
		 * 初始化地图 
		 */		
		private function initMaze():void
		{
			var mapData:Array = manager.getMapData();
			if(null == mapData || 0 == mapData.length)
				return;
			
			pathFinder.setMapData(mapData);
			elementLayer.removeAllChildren();
			var g:Graphics = meshShape.graphics;
			g.clear();
			g.lineStyle(1,0xff00ff);
			for (var i:int = 0; i <= mapData[0].length; i++) {
				g.moveTo(0, i*MapItem.HEIGHT);
				g.lineTo(MapItem.WIDTH*(mapData.length),  i*MapItem.HEIGHT);
			}
			for (var j:int = 0; j <= mapData.length; j++) {
				g.moveTo(j*MapItem.WIDTH, 0);
				g.lineTo(j*MapItem.WIDTH, MapItem.HEIGHT*(mapData[0].length));
			}
			for (var k:int = 0; k < mapData.length; k++) {
				for (var l:int = 0; l < mapData[0].length; l++){
					var mapItem:MapItem = new MapItem();
					mapItem.type = mapData[k][l];
					mapItem.move(k*MapItem.WIDTH, l*MapItem.HEIGHT);
					elementLayer.addChild(mapItem);
				}
			}
			drawPoint(manager.enterPoint,elementLayer,0xff00ff, 1);
			drawPoint(manager.exitPoint,elementLayer,0xff00ff, 1);
		}
		
		private function clearPath():void
		{
			pathLayer.removeAllChildren();
		}
		
		/**
		 * 测试寻路并把路径画出来 
		 * @param showImmediately 是否立刻展示（否则动画展示路径）
		 */		
		private function showFindPath(showImmediately:Boolean = false):void
		{
			_tmpFindPathArr = pathFinder.findPath(manager.enterPoint, manager.exitPoint);
			if(null == _tmpFindPathArr){
				Message.show("("+manager.enterPoint.x+","+manager.enterPoint.y+") -> ("+manager.exitPoint.x+","+manager.exitPoint.y+")没有找到合适路径");
				eventManager.dispatchToController(new ParamEvent(SceneEvent.PATH_SHOW_COMPLETE));
				return;
			}
			
			if(false == showImmediately){
				TimerManager.getInstance().removeItem(showPathTimerHandler);
				TimerManager.getInstance().addItem(50, showPathTimerHandler);
			}else{
				for (var i:int = 0; i < _tmpFindPathArr.length; i++) {
					var p:Point = _tmpFindPathArr[i];
					drawPoint(p, pathLayer, 0xff00ff, 0.5);
				}
			}
		}
		
		private function showPathTimerHandler():void
		{
			if(null == _tmpFindPathArr || 0 == _tmpFindPathArr.length){
				TimerManager.getInstance().removeItem(showPathTimerHandler);
				eventManager.dispatchToController(new ParamEvent(SceneEvent.PATH_SHOW_COMPLETE));
				return;
			}
			var p:Point = _tmpFindPathArr.shift();
			drawPoint(p, pathLayer, 0xff00ff, 0.5);
		}
		
		/**
		 * 画出路径点
		 * @param p
		 */		
		private function drawPoint(p:Point, parentLayer:BaseSprite, color:uint, _alpha:Number):void
		{
			var pathItem:Shape = new Shape();
			pathItem.graphics.beginFill(color,_alpha);
			pathItem.graphics.drawRect(1,1,MapItem.WIDTH, MapItem.HEIGHT);
			pathItem.graphics.endFill();
			pathItem.x = p.x*MapItem.WIDTH;
			pathItem.y = p.y*MapItem.HEIGHT;
			parentLayer.addChild(pathItem);
		}
		
		protected function onRestartDemoHandler(event:ParamEvent):void
		{
			clearPath();
			showFindPath();
		}
		
		/**
		 * 渲染主函数 
		 * @param step
		 */		
		public function rendering(step:int = 0):void
		{
			{ // 渲染所有玩家跟怪物
				for each (var player:Player in playerDic) 
				{
					player.rendering(step);
				}
				for each (var monster:Monster in monsterDic) 
				{
					player.rendering(step);
				}
			}
		}
		
		private function onCreatePlayerHandler(e:ParamEvent):void
		{
			var player:Player = new Player();
			player.playerVo = e.param as PlayerVo;
			player.move(manager.enterPoint.x*MapItem.WIDTH,manager.enterPoint.y*MapItem.HEIGHT);
			liveLayer.addChild(player);
			playerDic[player.playerVo.id] = player;
		}
		
		private function onCreateMonsterHandler(e:ParamEvent):void
		{
			// TODO Auto Generated method stub
			
		}
		
		private function onRebuildSceneHandler(e:ParamEvent):void
		{
			clearMaze();
			initMaze();
		}
		
		/**
		 * [继承] 恢复资源
		 * 
		 */		
		public override function resume():void
		{
			super.resume();
			manager.addEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
			manager.addEventListener(SceneEvent.REBUILD_MAP, onRebuildSceneHandler);
			manager.addEventListener(SceneEvent.CREATE_PLAYER, onCreatePlayerHandler);
			manager.addEventListener(SceneEvent.CREATE_MONSTER, onCreateMonsterHandler);
			
			RenderManager.getInstance().add(this);
		}
		
		/**
		 * [继承] 释放资源
		 * 
		 */		
		public override function dispose():void
		{
			super.dispose();
			manager.removeEventListener(SceneEvent.RESTART_DEMO, onRestartDemoHandler);
			manager.removeEventListener(SceneEvent.REBUILD_MAP, onRebuildSceneHandler);
			manager.removeEventListener(SceneEvent.CREATE_PLAYER, onCreatePlayerHandler);
			manager.removeEventListener(SceneEvent.CREATE_MONSTER, onCreateMonsterHandler);
			
			RenderManager.getInstance().remove(this);
		}
	}
}