using Unity.Netcode;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

// 플레이어들의 준비 상태를 관리하는 클래스
public class PlayerReadyState : NetworkBehaviour
{
    // 클라이언트 ID(ulong)를 키로, 준비 상태(bool)를 값으로 저장하는 딕셔너리
    private Dictionary<ulong, bool> _readyStates = new();

    // 싱글톤 인스턴스
    public static PlayerReadyState Instance;

    // 오브젝트 생성 시 호출되는 초기화 함수
    private void Awake()
    {
        // 싱글톤 패턴 적용: 인스턴스가 없으면 자신을 할당
        if (!Instance)
        {
            Instance = this;
        }

        // 기존 준비 상태 모두 초기화
        _readyStates.Clear();
    }

    // 서버에서 실행되는 RPC: 클라이언트가 준비 상태 제출 시 호출됨
    // RequireOwnership = false 이므로 소유자가 아니어도 호출 가능
    [ServerRpc(RequireOwnership = false)]
    public void SubmitReadyServerRpc(ServerRpcParams rpcParams = default)
    {
        // 준비 상태를 보낸 클라이언트 ID를 받아옴
        ulong clientId = rpcParams.Receive.SenderClientId;

        // 해당 클라이언트의 준비 상태를 true로 설정
        _readyStates[clientId] = true;

        // 모든 플레이어가 준비되었는지 체크하고, 준비되었으면 게임 시작
        if (AllPlayersReady())
        {
            InGameManager.Instance.StartGame();
        }
    }

    // 모든 플레이어가 준비 상태인지 확인하는 함수
    private bool AllPlayersReady()
    {
        // 준비 상태 딕셔너리에 저장된 플레이어 수가
        // 현재 네트워크에 연결된 클라이언트 수보다 적으면 false
        if (_readyStates.Count < NetworkManager.Singleton.ConnectedClients.Count)
        {
            return false;
        }

        // 플레이어 수가 1명 이하이면 false (2명 이상부터 게임 시작 가능)
        if (InGameManager.Instance.ConnectedUserNum <= 1)
        {
            return false;
        }

        // 모든 준비 상태가 true여야 true 반환 (모두 준비됨)
        return _readyStates.Values.All(isReady => isReady);
    }
}
