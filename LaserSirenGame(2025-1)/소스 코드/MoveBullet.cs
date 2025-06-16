using UnityEngine;

public class MoveBullet : MonoBehaviour
{
    [SerializeField] float speed;

    [SerializeField] LayerMask mask;

    private void Start()
    {
        Destroy(gameObject, 1);
    }

    void Update()
    {

        RaycastHit[] hits = Physics.RaycastAll(transform.position, -transform.forward, speed * Time.deltaTime, mask);
        
        foreach (RaycastHit hit in hits)
        {

            if (hit.transform.gameObject.layer == LayerMask.NameToLayer("Mole"))
            {
                //�δ������� ������
                if (hit.transform.gameObject.GetComponent<Mole>() != null)
                {
                    Mole mole = hit.transform.gameObject.GetComponent<Mole>();
                    mole.GetDamage(1);
                }
            }
        }
        transform.Translate(0, 0, -speed * Time.deltaTime);


        //�ʵ峪 �δ������� �ε����� ���
        if (hits.Length != 0)
        {
            Destroy(gameObject);
        }
    }


}
