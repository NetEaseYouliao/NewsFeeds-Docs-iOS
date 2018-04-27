//
//  WKWebViewJS.h
//  Pods
//
//  Created by neal on 2018/1/31.
//

var wk_tasks = {};
var wk_task_id = 0;

function Task(id, onSuccess, onError) {
    this.id = id;
    this.onSuccess = onSuccess;
    this.onError = onError;
}

function wk_call(method, params, onSuccess, onError) {
    var task = new Task(wk_task_id, onSuccess, onError);
    wk_tasks[wk_task_id] = task;
    wk_task_id++;
    window.webkit.messageHandlers.nejsbridge.postMessage({
                                                         id: task.id,
                                                         method: method,
                                                         params: params
                                                         });
};

function wk_notify(method, params) {
    window.webkit.messageHandlers.nejsbridge.postMessage({
                                                         method: method,
                                                         params: params
                                                         });
};

function wk_onSuccess(id, message) {
    var task = wk_tasks[id];
    task.onSuccess(message);
    delete wk_tasks[id]
}

function wk_onError(id, message) {
    var task = wk_tasks[id];
    task.onError(message);
    delete wk_tasks[id]
}

function wk_log(message) {
    window.webkit.messageHandlers.nejsbridge.postMessage({
                                                         method: 'log',
                                                         params: { log: message }
                                                         });
}

var wk_actions = {};

function wk_register(action, callback) {
    wk_actions[action] = callback;
}

function wk_invoke(action, id, params) {
    callback = wk_actions[action];
    if (typeof callback === 'function') {
        return callback(id, params);
    } else {
        wk_log("action" + action + "not found");
    }
}

//兼容老版本
window.jsonRPC = {};
window.jsonRPC.call = wk_call;
window.jsonRPC.notify = wk_notify;
window.jsonRPC.log = wk_log;
window.jsonRPC.register = wk_register;

