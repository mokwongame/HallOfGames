using Unity.Netcode;  // Unity Netcode for GameObjects ���̺귯���� ����Ͽ� ��Ʈ��ũ ��� ����
using UnityEngine;    // Unity ������ �⺻ ����� ����ϱ� ���� ���ӽ����̽�

// �� Ŭ������ ��Ʈ��ũ ȯ�濡�� ī�޶� �÷��̾ ���� �����̵��� ������
public class CameraManager : NetworkBehaviour
{
    [SerializeField]
    private GameObject player;  // ������ �÷��̾� ������Ʈ (�����Ϳ��� �Ҵ��ϰų� ��Ÿ�� �� ã��)

    public float sensitivity = 5f;  // ���콺 ȸ�� ����
    private bool _bIsDragging;      // ���� ���콺�� �巡�� ������ ����
    private float _currentAngle;    // ī�޶��� ���� ȸ�� ����(Y�� ����)

    // ī�޶��� �Ÿ� �� �� ������ (������)
    private const float Distance = 15f;     // �÷��̾�κ��� ī�޶������ �Ÿ�
    private const float Height = 5f;        // �÷��̾� ���� ī�޶� ����
    private const float FixedXAngle = 20f;  // ī�޶��� X��(����) ���� ����

    // �� �����Ӹ��� ȣ��Ǵ� Unity �⺻ �Լ�
    private void Update()
    {
        // ���� �÷��̾ �Ҵ���� �ʾҴٸ� �õ�
        if (!player)
        {
            TryAssignPlayer();
            return;  // �÷��̾ ������ ī�޶� ������Ʈ ����
        }

        HandleMouseInput();      // ���콺 �Է� ó��
        UpdateCameraPosition();  // ī�޶� ��ġ �� ȸ�� ����
    }

    // �÷��̾� ������Ʈ�� ���� �Ҵ���� ���� ��� �ڵ����� ã�Ƽ� �Ҵ�
    private void TryAssignPlayer()
    {
        // ��Ʈ��ũ �Ŵ��� �Ǵ� ���� �Ŵ����� �������� ������ �ߴ�
        if (!NetworkManager.Singleton || NetworkManager.Singleton.SpawnManager == null)
        {
            return;
        }

        // ���� ȣ��Ʈ(����)��� ù ��° ����� Ŭ���̾�Ʈ�� PlayerObject�� ������
        if (NetworkManager.Singleton.IsHost)
        {
            if (NetworkManager.Singleton.ConnectedClients.Count > 0)
            {
                player = NetworkManager.Singleton.ConnectedClients[0].PlayerObject?.gameObject;
            }
        }
        // Ŭ���̾�Ʈ��� �ڽ��� ���� PlayerObject�� ������
        else if (NetworkManager.Singleton.IsClient)
        {
            player = NetworkManager.Singleton.SpawnManager.GetLocalPlayerObject()?.gameObject;
        }

        // �⺻���� �ʱ� ī�޶� ȸ�� ���� (���鿡�� ������ ���ϴ� ����)
        transform.rotation = Quaternion.Euler(80, 0, 0);
    }

    // ���콺 �Է����� ī�޶� ȸ�� ������ ���� �� ó��
    private void HandleMouseInput()
    {
        // ���콺 ���� ��ư Ŭ�� ����
        if (Input.GetMouseButtonDown(0))
        {
            _bIsDragging = true;
        }

        // ���콺 ���� ��ư�� ����
        if (Input.GetMouseButtonUp(0))
        {
            _bIsDragging = false;
        }

        // ���콺�� �巡���ϴ� ���� X�� �����ӿ� ���� ī�޶� ȸ�� ������ ����
        if (_bIsDragging)
        {
            float mouseX = Input.GetAxis("Mouse X");  // ���콺�� ���� �̵���
            _currentAngle += mouseX * sensitivity;     // ȸ�� ������ ����
        }
    }

    // �÷��̾� ��ġ�� ���� ī�޶� ��ġ �� ȸ������ ����
    private void UpdateCameraPosition()
    {
        // �÷��̾� ������Ʈ�� ������ �������� ����
        if (!player)
        {
            return;
        }

        // ������ X�� ������ �巡�׷� ��ȭ�� Y�� ������ ������ ȸ�� ���ʹϾ�
        Quaternion rotation = Quaternion.Euler(FixedXAngle, _currentAngle, 0);

        // ȸ�� ���ʹϾ��� �������� �÷��̾� �޺κ�(Distance ��ŭ) ��ġ ���
        Vector3 offset = rotation * new Vector3(0, 0, -Distance);

        // ���� ī�޶� ��ġ = �÷��̾� ��ġ + �Ÿ� ���� + ���� ����
        transform.position = player.transform.position + offset + Vector3.up * Height;

        // ī�޶��� ȸ���� ���� ȸ�������� ����
        transform.rotation = rotation;
    }
}
